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
//TODO: fix the #if compilation stuff somehow.
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
	
	/*
	for (int s_i = 1; s_i < _BEACH_WIDTH +1; ++s_i)
	{
		s[s_i] = beach_sections[s_i-1];
	}
	*/

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

/* Local reward L : Calculates the reward for an individual beach section 
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
		/* For standard D */
		rewards.push_back(L(beach_sections[s]) - L(beach_sections[s] - 1));
#if (_TEST_TYPE == 'A')
		/* for accumulation */
		agents[a].setReward(agents[a].getReward() + rewards.back());
#endif
	}
}

void Beach::RunBeach()
{
	/* Sterilization and initialization */
	for (int i = 0; i < _BEACH_WIDTH; ++i)
	{
		beach_sections[i] = 0;
	}
	Init();
	for (int t = 0; t < _TIMESTEPS; ++t)
	{
		//Print();
		ExecuteTimeStep();
#if (_TEST_TYPE == 'A')
		std::vector<double> rewards;
		D(rewards);
		auto r_it = rewards.begin();
		for (auto it = agents.begin(); it != agents.end(); ++it)
		{
			it->setReward(it->getReward() + *r_it);
		}
#endif
	}
	//Print();
}

void Beach::Init()
{
	/* Agents no longer randomly initialized, 
	 * but rather all start at position N */
	int position = 3;
	for (int a_id = 0; a_id < _NUM_AGENTS; ++a_id)
	{
		agents[a_id].setPos(position);
		++beach_sections[position];
	}
}

void Beach::Print()
{
	
	std::ofstream out;
	out.open("best_config.out", std::ofstream::app);

	out << std::endl;
	for (int section=0; section < _BEACH_WIDTH; ++section)
	{
		out << "~";
		for (int a=0; a < beach_sections[section]; ++a)
		{
			out << "+";
		}
		out << std::endl;
	}
	out.close();
}

void Beach::setAgents(std::vector<Agent> agents)
{
	assert(this->agents.size() == 0);
	assert(agents.size() == _NUM_AGENTS);
	this->agents = agents;
}

void Beach::clearAgents() { this->agents.clear(); }

void Beach::RewardAgents(std::ofstream &d_out, std::ofstream &g_out, double &reward_max)
{
	std::vector<double> rewards;
	double max = -DBL_MAX;
#if (_TEST_TYPE == 'D' || _TEST_TYPE == 'A')
	this->D(rewards);
	std::cout << "Rewards: [";
	for (int a = 0; a < _NUM_AGENTS; ++a)
	{
		std::cout << rewards[a] << " ";
#if (_TEST_TYPE == 'D')
		this->agents[a].setReward(rewards[a]);
#endif
#if (_TEST_TYPE == 'A')
		this->agents[a].setReward(rewards[a] + this->agents[a].getReward());
#endif
		if (max < rewards[a])
		{
			max = rewards[a];
		}
	}
#endif
	std::cout << std::endl;
	std::cout << "Max reward: " << max << std::endl;
	d_out << max << std::endl;;
	
	/* comparision to G */
	rewards.clear();
	this->G(rewards);
	std::cout << "VS G: " << rewards[0] << std::endl;
	g_out << rewards[0] << std::endl;

	if (reward_max < rewards[0])
	{
		reward_max = rewards[0];
		Print();
	}

#if (_TEST_TYPE == 'G')
	/* reward with G, not d */
	for (auto it = agents.begin(); it != agents.end(); ++it)
	{
		it->setReward(rewards[0]);
	}
#endif
	

#ifdef DEBUG
	/* debugging info */
	//std::cout << " INFO: " << agents.size() <<  std::endl;
#endif
}

void Beach::extractAgents(std::vector<Agent> &other)
{
	other = this->agents;
}

