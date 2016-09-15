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

std::vector<int> Beach::GetAgentActions()
{
	// currently unused
}


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

double Beach::G()
{
	return 0;
}

void Beach::D(std::vector<double> &rewards)
{
	double g = this->G();
	for (int a = 0; a < NUM_AGENTS; ++a)
	{
		rewards.push_back(rand()%50); // TODO add a real calculation of D
	}
}

void Beach::RunBeach()
{
	for (int t = 0; t < TIMESTEPS; ++t)
	{
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
	std::cout << "num_agents == agents.size: " << num_agents << " "<<agents.size()<<std::endl;
	std::cout << "agent positions: [";
	for (int a_id=0; a_id < num_agents; ++a_id)
	{
		std::cout << agents[a_id].getPos() << " ";
	}
	std::cout << "]" << std::endl;
	std::cout << "Beach: [";
	for (int i=0; i < BEACH_WIDTH; ++i)
	{
		std::cout << beach_sections[i] << " ";
	}
	std::cout << "]" << std::endl;
}

int Beach::getnum_agents() { return num_agents; }

void Beach::setAgents(std::vector<Agent> agents)
{
	assert(this->agents.size() == 0);
	assert(agents.size() == NUM_AGENTS);
	this->agents = agents;
}

void Beach::clearAgents() { this->agents.clear(); }

void Beach::RewardAgents()
{
	std::vector<double> rewards;
	this->D(rewards);
	for (int a = 0; a < NUM_AGENTS; ++a)
	{
		this->agents[a].setReward(rewards[a]);
	}
}

void Beach::extractAgents(std::vector<Agent> &other)
{
	other = this->agents;
}

