/*********************************************************************
*  beach.h
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

#ifndef _BEACH_H
#define _BEACH_H

#include <vector>
#include <iostream>
#include <cstdlib>
#include <float.h>
#include <fstream>

#include <fann.h>
#include <fann_cpp.h>

#include "config.h"
#include "agent.h"
#include "state.h"

class Beach
{
	private:
		/* Members */
		std::vector<Agent> agents;
		int beach_sections[BEACH_WIDTH];
		int num_agents;

		/* Methods */
		//std::vector<int> GetAgentActions();
		State CalculateState(int agent_id);
		void ExecuteTimeStep();
		void G(std::vector<double> &);
		void D(std::vector<double> &);
		double L(double);

	public:
		/* Methods */
		Beach();
		void RunBeach();
		void RandomInit();
		void Print();
		int getnum_agents();
		void setAgents(std::vector<Agent>);
		void clearAgents();
		void RewardAgents(std::ofstream&);
		void extractAgents(std::vector<Agent> &);
};

#endif
