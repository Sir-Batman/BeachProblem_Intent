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
	for (int i = 0; i < _BEACH_WIDTH; ++i)
	{
		this->beach_sections[i] = 0;
	}

	this->agents.reserve(_NUM_AGENTS);
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
	
	for (int s_i = 1; s_i < _BEACH_WIDTH +1; ++s_i)
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
	int actions[_NUM_AGENTS];
	for (int a_id = 0; a_id < _NUM_AGENTS; ++a_id)
	{
		State s_i = CalculateState(a_id);
		actions[a_id] = agents[a_id].nextAction(s_i);
	}

	/* Move agents on beach */
	for (int a_id = 0; a_id < _NUM_AGENTS; ++a_id)
	{
		/* calculate proper new position, with bounds checking. 
		 * Subtract 1 to translate action to position modifier */
		int pos_new = agents[a_id].getPos() + actions[a_id] - 1;
		if (pos_new < 0 || pos_new == _BEACH_WIDTH)
		{
			pos_new = agents[a_id].getPos();
		}

		//std::cout << "OLD : " << pos_new << beach_sections[pos_new] << std::endl;
		//std::cout << "      " << agents[a_id].getPos() << beach_sections[agents[a_id].getPos()] << std::endl;
		// update positions
		++beach_sections[pos_new];
		--beach_sections[agents[a_id].getPos()];
		agents[a_id].setPos(pos_new);
		//std::cout << "NEW : " << pos_new <<" "<< beach_sections[pos_new] << std::endl;
		//std::cout << "      " << agents[a_id].getPos() <<" "<< beach_sections[agents[a_id].getPos()] << std::endl;
	}
	// TODO Log information?
}

/* Calculates the reward for an individual beach section 
 * given the number of people on the section of the beach.
 * PSI defined in config.h */
double Beach::L(double x_t)
{
	return x_t * (exp(-1*x_t / _PSI));
}

void Beach::G(std::vector<double> &rewards)
{
	double total = 0;
	for (int s = 0; s < _BEACH_WIDTH; ++s)
	{
		/* Add the reward for section S to the total */
		total += L(this->beach_sections[s]);
	}
	if (rewards.size() != 0)
	{
		std::cerr << "Rewards vector non-zero length. Clearing..." << std::endl;
		rewards.clear();
	}
	for (int a = 0; a < _NUM_AGENTS; ++a)
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
	for (int a = 0; a < _NUM_AGENTS; ++a)
	{
		int s = agents[a].getPos();
		rewards.push_back(L(beach_sections[s]) - L(beach_sections[s] - 1));
	}
}

void Beach::RunBeach()
{
	/* Sterilization and initialization */
	for (int i = 0; i < _BEACH_WIDTH; ++i)
	{
		beach_sections[i] = 0;
	}
	RandomInit();
	for (int t = 0; t < _TIMESTEPS; ++t)
	{
		//Print();
		ExecuteTimeStep();
	}
	Print();

}

void Beach::RandomInit()
{
	for (int a_id = 0; a_id < _NUM_AGENTS; ++a_id)
	{
		int position = rand() % _BEACH_WIDTH;
		agents[a_id].setPos(position);
		++beach_sections[position];
	}
}

void Beach::Print()
{
	std::cout << std::endl;
	for (int section=0; section < _BEACH_WIDTH; ++section)
	{
		std::cout << "~";
		for (int a=0; a < beach_sections[section]; ++a)
		{
			std::cout << "+";
		}
		std::cout << std::endl;
	}
}

void Beach::setAgents(std::vector<Agent> agents)
{
	assert(this->agents.size() == 0);
	assert(agents.size() == _NUM_AGENTS);
	this->agents = agents;
}

void Beach::clearAgents() { this->agents.clear(); }

void Beach::RewardAgents(std::ofstream &reward_out)
{
	std::vector<double> rewards;
	this->D(rewards);
	double max = -DBL_MAX;
	std::cout << "Rewards: [";
	for (int a = 0; a < _NUM_AGENTS; ++a)
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
	
	/* comparision to G */
	rewards.clear();
	this->G(rewards);
	std::cout << "VS G: " << rewards[0] << std::endl;

#ifdef DEBUG
	/* debugging info */
	std::cout << " INFO: " << agents.size() <<  std::endl;
#endif


}

void Beach::extractAgents(std::vector<Agent> &other)
{
	other = this->agents;
}

