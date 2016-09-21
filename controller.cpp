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

	policies.reserve(_NUM_AGENTS * _NUM_BEACHES);
	auto it = policies.begin();
	for (int i = policies.size(); i < _NUM_AGENTS * _NUM_BEACHES; ++i) {
		mutateNet = mutate(*it);
		policies.push_back(mutateNet);
		++it;
	}
}


void createPolicies(struct netConfig NC, std::vector<FANN::neural_net *> &policies)
{
	for (int i = 0; i < _NUM_AGENTS * _NUM_BEACHES; i++)
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
	NC.num_layers = _NUMBER_OF_LAYERS;
	NC.layers = new unsigned int[_NUMBER_OF_LAYERS];
	NC.layers[0] = _NN_INPUT_LAYER;
	NC.layers[1] = 18;
	NC.layers[2] = 9;
	NC.layers[3] = _NN_OUT_LAYER;
	NC.randWeights = _RANDOM_WEIGHTS;
	NC.randMin = _RANDOM_NET_MIN;
	NC.randMax = _RANDOM_NET_MAX;

	srand(time(NULL));

	std::vector<FANN::neural_net *> policies;
	createPolicies(NC, policies);

	std::vector<Beach> coastline(_NUM_BEACHES);

	//auto engine = std::mt19937{std::random_device{}()};

	/* Open Logging files */
	std::ofstream reward_file;
	reward_file.open("data/max_rewards_1.csv");
	std::vector<Agent> agents;
	for (int gen = 0; gen < _NUM_GENERATIONS; ++gen)
	{
		std::cout << "BEGINING GENERATION " <<  gen << std::endl;
		/* Make random teams of agents */
		/* Create Agents */
		agents.clear();
		agents.reserve(policies.size());
		//std::shuffle(policies.begin(), policies.end(), engine);
		std::random_shuffle(policies.begin(), policies.end());
		for (auto p_it = policies.begin(); p_it != policies.end(); ++p_it)
		{
			agents.emplace_back(*p_it);
		}
		/* and then create teams */
		std::vector<Agent> team;
		team.reserve(_NUM_AGENTS);
		for (int i = 0; i < _NUM_BEACHES; ++i)
		{
			team.clear();
			for (int a = 0; a < _NUM_AGENTS; ++a)
			{
				team.emplace_back(policies[ (_NUM_AGENTS * i) + a ]);
			}
			/* Give teams to a beach */
			coastline[i].clearAgents();
			coastline[i].setAgents(team);
		}

		// made a seperate loop for potential threading in the future
		/* Run a day on the beach and get rewards */
		agents.clear();
		agents.reserve(_NUM_AGENTS * _NUM_BEACHES);

		//team.clear();
		for (int i = 0; i < _NUM_BEACHES; ++i)
		{
			coastline[i].RunBeach();
			coastline[i].RewardAgents(reward_file);

			/* Recreate a sortable version of the agents. */
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

		/* Remove policies from successful agents */
		/* useless from call inside previous loop
		for (auto a_it = agents.begin()+(agents.size()/2); a_it != agents.end(); ++a_it)
		{
			a_it->setAddress(NULL);
		}
		*/
		/* Policies of lower half are destroyed. Now to make new ones and mutate */
		doublePopulation(policies);
	} // END GENERATION LOOP

	destroyPolicies(policies);
	delete [] NC.layers;
	return 0;
}
