/*********************************************************************
*  beach.cpp
*
*  TODO add description
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

#include "beach.h"

Beach::Beach()
{
	this->num_agents = NUM_AGENTS; // From config.h

	for (int i = 0; i < BEACH_WIDTH; ++i)
	{
		this->beach_sections[i] = 0;
	}

	this->agents.reserve(NUM_AGENTS);
}

/*
std::vector<int> Beach::GetAgentActions()
{
	// currently unused
}
*/


State Beach::CalculateState(int agent_id)
{
	State s;
	s[SELF_POS] = agents[agent_id].getPos();
	
	for (int s_i = 1; s_i < BEACH_WIDTH +1; ++s_i)
	{
		s[s_i] = beach_sections[s_i-1];
	}

	return s;
}

void Beach::ExecuteTimeStep()
{
#ifdef DEBUG
	assert(agents.size() != 0);
#endif
	
	/* Get agent actions */
	int actions[NUM_AGENTS];
	for (int a_id = 0; a_id < NUM_AGENTS; ++a_id)
	{
		State s_i = CalculateState(a_id);
		actions[a_id] = agents[a_id].nextAction(s_i);
	}

	/* Move agents on beach */
	for (int a_id = 0; a_id < NUM_AGENTS; ++a_id)
	{
		// calculate proper new position, with bounds checking
		int pos_new = agents[a_id].getPos() + actions[a_id];
		if (pos_new < 0)
		{
			pos_new = 0;
		}
		else if (pos_new > BEACH_WIDTH)
		{
			pos_new = BEACH_WIDTH;
		}

		// update positions
		++beach_sections[pos_new];
		--beach_sections[agents[a_id].getPos()];
		agents[a_id].setPos(pos_new);
	}
	// TODO Log information?
}

/* Calculates the reward for an individual beach section 
 * given the number of people on the section of the beach.
 * PSI defined in config.h */
double Beach::L(double x_t)
{
	return x_t * (exp(-1*x_t / PSI));
}

void Beach::G(std::vector<double> &rewards)
{
	double total = 0;
	for (int s = 0; s < BEACH_WIDTH; ++s)
	{
		/* Add the reward for section S to the total */
		total += L(this->beach_sections[s]);
	}
	if (rewards.size() != 0)
	{
		std::cerr << "Rewards vector non-zero length. Clearing..." << std::endl;
		rewards.clear();
	}
	for (int a = 0; a < NUM_AGENTS; ++a)
	{
		rewards.push_back(total);
	}
}

void Beach::D(std::vector<double> &rewards)
{
	if (rewards.size() != 0)
	{
		std::cerr << "Rewards vector non-zero length. Clearing..." << std::endl;
		rewards.clear();
	}
	for (int a = 0; a < NUM_AGENTS; ++a)
	{
		int s = agents[a].getPos();
		rewards.push_back(L(beach_sections[s]) - L(beach_sections[s] - 1));
	}
}

void Beach::RunBeach()
{
	/* Sterilization and initialization */
	for (int i = 0; i < BEACH_WIDTH; ++i)
	{
		beach_sections[i] = 0;
	}
	RandomInit();
	for (int t = 0; t < TIMESTEPS; ++t)
	{
		//Print();
		ExecuteTimeStep();
	}
}

void Beach::RandomInit()
{
	for (int a_id = 0; a_id < NUM_AGENTS; ++a_id)
	{
		int position = rand() % BEACH_WIDTH;
		agents[a_id].setPos(position);
		++beach_sections[position];
	}
}

void Beach::Print()
{
	std::cout << std::endl;
	for (int section=0; section < BEACH_WIDTH; ++section)
	{
		std::cout << "~";
		for (int a=0; a < beach_sections[section]; ++a)
		{
			std::cout << "+";
		}
		std::cout << std::endl;
	}
}

int Beach::getnum_agents() { return num_agents; }

void Beach::setAgents(std::vector<Agent> agents)
{
	assert(this->agents.size() == 0);
	assert(agents.size() == NUM_AGENTS);
	this->agents = agents;
}

void Beach::clearAgents() { this->agents.clear(); }

void Beach::RewardAgents(std::ofstream &reward_out)
{
	std::vector<double> rewards;
	this->D(rewards);
	double max = -DBL_MAX;
	std::cout << "Rewards: [";
	for (int a = 0; a < NUM_AGENTS; ++a)
	{
		std::cout << rewards[a] << " ";
		this->agents[a].setReward(rewards[a]);
		if (max < rewards[a])
		{
			max = rewards[a];
		}
	}
	std::cout << std::endl;
	std::cout << "Max reward: " << max << std::endl;
	reward_out << max << std::endl;;
}

void Beach::extractAgents(std::vector<Agent> &other)
{
	other = this->agents;
}

