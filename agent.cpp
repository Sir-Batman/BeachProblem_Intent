/*********************************************************************
*  agent.cpp
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

#include "agent.h"

/* The default copy constructor makes things unsusable at start,
 * since values such as the NN are to be set by outside factors.
 * This class serves as the controlling wrapper, not the setting wrapper.
 */
Agent::Agent()
{
	this->pos = -1; 
	this->reward = 0;
	this->policy = NULL;
}

/* Initialize the position to the value given, and
 * set the net to point to the one given. */
Agent::Agent(FANN::neural_net *policy)
{
	this->pos = -1;
	this->reward = 0;
	this->policy = policy;
}

//  copy constructor TODO figure out how much changing this needs
Agent::Agent(const Agent& that)
{
	//std::cout << "COPY CONSTRUCTOR!!!" << std::endl;
	this->pos = that.pos;
	this->reward = that.reward;
	this->policy = that.policy;
}

// copy assignment operator TODO look at FANN::neural_net * in here.
Agent& Agent::operator=(const Agent& that)
{
	//std::cout << "EQUAL OPERATOR!!!" << std::endl;
	this->pos = that.pos;
	this->reward = that.reward;
	this->policy = that.policy;
    return *this;
}

/* The next action is chosen from three directions.
 * Left, stay, and right. 0 = left, 1 = center, 2 = right */

int Agent::nextAction(State s)
{
	/* Picks the output from the neural net */
	fann_type* output = policy->run( (fann_type*) s.array);

	int max_i = 0;
	for (int i = 0; i < _NN_OUT_LAYER; ++i)
	{
		if (output[i] > output[max_i]) { max_i = i; }
	}

	return max_i;
}

int Agent::getPos() { return this->pos; }

void Agent::setPos(int pos) { this->pos = pos; }

double Agent::getReward()
{
	//TODO rewards will need restructuring
	/* This calculates a quick and dirty form of D.
	 * It returns the scaled reward based on how many
	 * POI the agent carried in its lifetime.
	 * This is the same as G(v)-G(v-i). */
	//return (double)(this->numCarried * 5)/2.0 + this->reward;
	return this->reward;
}

void Agent::incReward(double inc)
{
	this->reward += inc;
}

void Agent::setReward(double r)
{
	this->reward = r;
}

FANN::neural_net* Agent::getPolicyAddress()
{
	return this->policy;
}

bool Agent::operator< (const Agent &that)
{
	return this->reward < that.reward;
}

void Agent::setAddress(FANN::neural_net *p) { this->policy = p; }
