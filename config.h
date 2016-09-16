/*********************************************************************
*  config.h
*
*  This file creates precompiler variables and structs that the
*  main files will use to run.
*  This is editable in order to change the running of the program in one 
*  convienent place.
*
*  Copyright (C) 2016 Connor Yates
*
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

#ifndef _CONFIG_H
#define _CONFIG_H

/* Variable definitions */
#define NUM_GENERATIONS 100000
#define TIMESTEPS 50
#define NUM_AGENTS 30 // Defines the number of agents on a beach
#define NUM_BEACHES 5
#define BEACH_WIDTH 6
#define PSI 10 // Defines the optimal capacity of people on a section of a beach.

/* nn variables */
#define NUMBER_OF_LAYERS 4
#define RANDOM_WEIGHTS true
#define RANDOM_NET_MIN -10.0
#define RANDOM_NET_MAX 10.0
#define NN_INPUT_LAYER BEACH_WIDTH+1
#define NN_OUT_LAYER 3

/* FANN */
#include <fann.h>
#include <fann_cpp.h>

#include <string>

// struct representation of the gridworld configuration TODO need this?
struct gridConfig
{
	int numAgents;
	int numPOI;
	int width;
	int height;
	int poiWeight;
};

// struct representation of the neural net configuration 
struct netConfig
{
	FANN::network_type_enum net_type;
	unsigned int num_layers;
	unsigned int *layers;
	bool randWeights;
	float randMin;
	float randMax;
};


#endif
