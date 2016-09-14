/*********************************************************************
*  state.h
*
*  This struct is a wrapper around an agent's state,
*  allowing it to be treated as a singular object
*  (with additional bracketing being overloaded, if it is desired)
*  and encapsulates the int array away.
*  The internal array can be directly accessed if needed 
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

#ifndef _STATE_H
#define _STATE_H

#include "config.h"

/* Define the indicies to be used by the state variable globally, so everyone
 * uses the proper index */

#define SELF_POS 0

typedef struct State {

	double& operator[](int i) { return array[i]; }
	double array[BEACH_WIDTH+1];

} State;

#endif
