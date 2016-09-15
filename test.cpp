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

/* Testing libraries */
#include <assert.h>
#include "agent.h"
#include "beach.h"

/* general libraries */
#include <fann.h>
#include <fann_cpp.h>
#include <string>
#include <iostream>
#include <vector>
#include <cstdlib>

void TestBeach()
{
	Beach b;
	assert(b.getnum_agents() == NUM_AGENTS);
	
	// create agents
	std::vector<Agent> agents;
	for (int i = 0; i < NUM_AGENTS; ++i)
	{
		agents.push_back(Agent());
	}

	b.setAgents(agents);
	b.Print();
	b.RandomInit();
	b.RunBeach();
	b.Print();
}


int main()
{
	srand(time(NULL));
	TestBeach();

	return 0;
}
