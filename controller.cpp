/*********************************************************************
*  test.cpp
*
*  Contains unit tests and a main for running them.
*  Tests the rest of the code base.
*
*  Copyright (C) 2016 Connor Yates

*  This program is free software: you can redistribute it and/or modify
*  it under the terms of the GNU General Public License as published by
*  the Free Software Foundation, either version 3 of the License, or
*  (at your option) any later version.
*
*  This program is distributed in the hope that it will be useful,
*  but WITHOUT ANY WARRANTY; without even the implied warranty of
*  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
*  GNU General Public License for more details.
*
*  You should have received a copy of the GNU General Public License
*  along with this program.  If not, see <http://www.gnu.org/licenses/>.
*********************************************************************/

#include "controller.h"

/* Performs a mutation of 10% of the weights in the network, 
 * and returns a new, mutated version of the net parameter.
 * The origional is not changed.*/

FANN::neural_net* mutate(FANN::neural_net* net)
{
	double percent = .1;
	int length = net->get_total_connections();
	int numMutations = percent*length;

	FANN::neural_net* copy = new FANN::neural_net(*net);

	FANN::connection connections[length];
	net->get_connection_array(connections);

	/* mutate a percentage of random weight */
	for (int i = 0; i < numMutations; i++) {
		int index = rand() % length;
		int sign = rand() % 2 ? 1 : -1;
		fann_type current = connections[index].weight;
		double random = ((double)rand()/((double)RAND_MAX+1.0))/10.0;

		connections[index].weight += (fann_type) sign*random*current;
	}

	copy->set_weight_array(connections, length);

	return copy;
}

void doublePopulation(std::vector<FANN::neural_net*> &policies ) {

	FANN::neural_net* mutateNet = NULL;

	policies.reserve((policies.size() * 2) + 1);
	auto it = policies.begin();
	int size = policies.size();
	for (int i = 0; i < size; ++i) {
		mutateNet = mutate(*it);
		policies.push_back(mutateNet);
		++it;
	}
}


void createPolicies(struct netConfig NC, std::vector<FANN::neural_net *> &policies)
{
	for (int i = 0; i < NUM_AGENTS * NUM_BEACHES; i++)
	{
		FANN::neural_net* newNet = new FANN::neural_net(NC.net_type, NC.num_layers, NC.layers);

		if (NC.randWeights)
		{
			newNet->randomize_weights(NC.randMin, NC.randMax);
		}
		policies.push_back(newNet);
	}
}

void destroyPolicies(std::vector<FANN::neural_net *> &policies)
{
	for (auto it = policies.begin(); it != policies.end(); ++it)
	{
		delete *it;
	}
	policies.clear();
}

int main()
{
	/* Set up initial net configuration */
	struct netConfig NC;
	NC.net_type = FANN::LAYER;
	NC.num_layers = NUMBER_OF_LAYERS;
	NC.layers = new unsigned int[NUMBER_OF_LAYERS];
	NC.layers[0] = NN_INPUT_LAYER;
	NC.layers[1] = 18;
	NC.layers[2] = 9;
	NC.layers[3] = NN_OUT_LAYER;
	NC.randWeights = RANDOM_WEIGHTS;
	NC.randMin = RANDOM_NET_MIN;
	NC.randMax = RANDOM_NET_MAX;

	srand(time(NULL));

	std::vector<FANN::neural_net *> policies;
	createPolicies(NC, policies);

	std::vector<Beach> coastline(NUM_BEACHES);

	//auto engine = std::mt19937{std::random_device{}()};

	std::vector<Agent> agents;
	for (int gen = 0; gen < NUM_GENERATIONS; ++gen)
	{
		std::cout << "BEGINING GENERATION " <<  gen << std::endl;
		/* Make random teams of agents */
		/* Create Agents */
		agents.clear();
		//std::shuffle(policies.begin(), policies.end(), engine);
		std::random_shuffle(policies.begin(), policies.end());
		for (auto p_it = policies.begin(); p_it != policies.end(); ++p_it)
		{
			agents.emplace_back(*p_it);
		}
		/* and then create teams */
		std::vector<Agent> team;
		for (int i = 0; i < NUM_BEACHES; ++i)
		{
			team.clear();
			for (int a = 0; a < NUM_AGENTS; ++a)
			{
				team.push_back(Agent(policies[ (NUM_AGENTS * i) + a ]));
			}
			/* Give teams to a beach */
			coastline[i].clearAgents();
			coastline[i].setAgents(team);
		}

		// made a seperate loop for potential threading in the future
		/* Run a day on the beach and get rewards */
		agents.clear();
		agents.reserve(NUM_AGENTS * NUM_BEACHES);

		for (int i = 0; i < NUM_BEACHES; ++i)
		{
			coastline[i].RunBeach();
			coastline[i].RewardAgents();

			/* Recreate a sortable version of the agents. */
			std::vector<Agent> team;
			coastline[i].extractAgents(team);
			agents.insert(agents.end(), team.begin(), team.end());
		}
		std::sort(agents.begin(), agents.end());

		/* kill the older agents */
		for (auto a_it = agents.begin(); a_it != agents.begin()+(agents.size()/2); ++a_it)
		{
			FANN::neural_net * dead_policy = a_it->getPolicyAddress();
			a_it->setAddress(NULL);
			/* Find and destroy the net from policies */
			auto find = policies.begin();
			while ( (*find != dead_policy) && find != policies.end() )
			{
				++find;
			}
			if (find != policies.end())
			{
				policies.erase(find);
			}
			else
			{
				std::cout << "Policy not found" << std::endl;
			}
			delete dead_policy;
		}
		/* Policies of lower half are destroyed. Now to make new ones and mutate */
		doublePopulation(policies);
	} // END GENERATION LOOP

	// run generation \in generations
	// 		make teams of agents
	// 		give teams to beaches
	// 		run time steps on beach
	// 		calculate rewards of agents
	// 		sort agents
	// 		kill
	// 		copy and mutate
	// cleanup
	// 		destryo policies
	// 		close files
	//

	destroyPolicies(policies);
	delete [] NC.layers;
	return 0;
}
