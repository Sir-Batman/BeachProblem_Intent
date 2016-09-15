/*********************************************************************
*  agent.h
*
*  This class is an object representation of the agents on a beach 
*  trying to find a place to sit.  
*
*  NOTE: This class does not perform memory management on the policies.
*
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

#ifndef _AGENT_H
#define _AGENT_H

class POI; 

#include <deque>
#include <iostream>
#include <cstdlib>
#include <fann.h>
#include <fann_cpp.h>
//#include "config.h"
//#include "position.h"
//#include "poi.h"
#include "state.h"
//#include "config.h"
//#include "Home.h"

/* enumeration of action behaviors */
enum
{
	LEFT, 
	NONE, 
	RIGHT
};

class Agent {

	private:
		//TODO add id?
		//bool carrying;       // is the agent carrying a poi?
		//int numCarried;     //  has the agent ever carried a poi?
		int pos;          // current position in world
		//POI* holding;        // points to the poi an agent is carrying (or NULL)
		double reward;
		
		FANN::neural_net *policy;

	public:	
		//  constructors
		Agent();
		Agent(FANN::neural_net *);
		//  copy constructor
		Agent(const Agent&);
		// copy assignment operator
		Agent& operator=(const Agent&);

		//  get the next action for the agent based on the
		//  current state and the neural net
		int nextAction(State);

		double getReward();

		//void hasCarried();

		int numberCarried();

		void incReward(double);
		
		// Set the internal position
		void setPos(int);
		
		// Get the internal Position
		int getPos();

		void setReward(double);

		FANN::neural_net* getPolicyAddress();

		bool operator<(const Agent&);
		
		void setAddress(FANN::neural_net *);
};

#endif
