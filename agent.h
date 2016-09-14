/*********************************************************************
*  agent.h
*
*  This class is an object representation of the agents on a beach 
*  trying to find a place to sit.  
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

#ifndef AGENT_INCLUDED
#define AGENT_INCLUDED

class POI; 

#include <deque>
#include <iostream>
#include <cstdlib>
#include <fann.h>
#include <fann_cpp.h>
#include "config.h"
//#include "position.h"
//#include "poi.h"
#include "state.h"
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
		bool carrying;       // is the agent carrying a poi?
		int numCarried;     //  has the agent ever carried a poi?
		Position p;          // current position in world
		POI* holding;        // points to the poi an agent is carrying (or NULL)
		double reward;

	public:	
		//  constructors
		Agent();
		Agent(bool, POI*, Position);
		//  copy constructor
		Agent(const Agent&);
		// copy assignment operator
		Agent& operator=(const Agent&);

		//  get the next action for the agent based on the
		//  current state and the neural net
		int nextAction(State, Position, Home, FANN::neural_net*);

		//  is the agent carrying anything?
		bool isCarrying();
		//  set the carrying signal appropriately
		void setCarrying(bool);

		//  return the POI the agent is holding
		POI* getHoldingPOI();
		//  set the POI the agent is holding
		void setHoldingPOI(POI*);

		Position getP();
		void setP(Position);

		double getReward();

		void hasCarried();

		int numberCarried();

		void incReward(double);

};

#endif
