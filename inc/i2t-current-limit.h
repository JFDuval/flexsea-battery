/****************************************************************************
	[Project] FlexSEA: Flexible & Scalable Electronics Architecture
	[Sub-project] 'Battery' Battery monitoring & protection
	Copyright (C) 2016 Dephy, Inc. <http://dephy.com/>

	This program is free software: you can redistribute it and/or modify
	it under the terms of the GNU General Public License as published by
	the Free Software Foundation, either version 3 of the License, or
	(at your option) any later version.

	This program is distributed in the hope that it will be useful,
	but WITHOUT ANY WARRANTY; without even the implied warranty of
	MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
	GNU General Public License for more details.

	You should have received a copy of the GNU General Public License
	along with this program.  If not, see <http://www.gnu.org/licenses/>.
*****************************************************************************
	[Lead developper] Jean-Francois (JF) Duval, jfduval at dephy dot com.
	[Origin] Based on Jean-Francois Duval's work at the MIT Media Lab 
	Biomechatronics research group <http://biomech.media.mit.edu/>
	[Contributors] 
*****************************************************************************
	[This file] I2t current limit
*****************************************************************************
	[Change log] (Convention: YYYY-MM-DD | author | comment)
	* 2016-09-14 | jfduval | New file
	*
****************************************************************************/

#ifndef INC_I2TCURRENT_H
#define INC_I2TCURRENT_H

//****************************************************************************
// Include(s)
//****************************************************************************

#include "main.h"

//****************************************************************************
// Public Function Prototype(s):
//****************************************************************************

void i2t_sample(int32 lastCurrentRead);
int i2t_compute(void);

//****************************************************************************
// Definition(s):
//****************************************************************************

#define I2T_SAMPLES				8
#define I2T_SAMPLES_SHIFT		3
//The algorithm uses 8bit values: we need to scale down the maximum current
//accordingly.
//Ex.: is you read ±11bits (±2048), 2048/256 = 8, so shift 3
#define I2C_SCALE_DOWN_SHIFT	3
//We use a leaky integrator. The leak should be set to the maximum sustained
//current required by your application. It's applied on the squared value (so 
//max current = 65536). Pick a fraction of that number. Ex.: if the max current
//that your sensor can read is 30A and you wan to support 10A continuous, 
//use (10/30)*65536 = 21845. With 10A flowing, your integrator will stay at 0.
//Anything above it will increase its count.
#define I2T_LEAK				21845
//What current limit do you want? 
//Limit = (TIME_AT_LIMIT_CURR / dt) * ( 65536*(LIMIT_CURR/MAX_MEASURABLE) - I2T_LEAK )
//Ex.: 15A for 1s 
//	Limit = (1s / 100ms) * ( 65536*(15A/30A) - 21845) = 109230.
#define I2T_LIMIT				109230
#define I2T_WARNING				(0.8*I2T_LIMIT)

//Return values:
#define RET_I2T_NORMAL			0
#define RET_I2T_WARNING			1
#define RET_I2T_LIMIT			2

#endif 	//INC_I2TCURRENT_H
