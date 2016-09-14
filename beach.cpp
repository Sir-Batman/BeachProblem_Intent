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
}

double Beach::D(double g)
{
}

void Beach::RunBeach()
{
	for (int t = 0; t < TIMESTEPS; ++t)
	{
		ExecuteTimeStep();
	}
}


