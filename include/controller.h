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

#ifndef _CONTROLLER_H
#define _CONTROLLER_H

#include <vector>
#include <iostream>
#include <cstdlib>
#include <algorithm>
#include <random>

#include <fann.h>
#include <fann_cpp.h>

#include "agent.h"
#include "beach.h"
#include "config.h"

/* Prototypes */
void destroyPolicies(std::vector<FANN::neural_net *> &policies);

#endif
