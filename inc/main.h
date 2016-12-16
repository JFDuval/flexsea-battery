/****************************************************************************
	[Project] FlexSEA: Flexible & Scalable Electronics Architecture
	[Sub-project] 'battery': Battery Protection & Monitoring
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
	[This file] main: Main
*****************************************************************************
	[Change log] (Convention: YYYY-MM-DD | author | comment)
	* 2016-09-20 | jfduval | Added proper header
	*
****************************************************************************/

#ifndef MAIN_H_
#define MAIN_H_
	
//****************************************************************************
// Include(s)
//****************************************************************************

#include <project.h>
#include <stdlib.h>
#include "isr_callbacks.h"
#include "main_fsm.h"
#include "misc.h"
#include "sensors.h"
#include "comm.h"
#include "fsm.h"
#include "i2t-current-limit.h"
#include "rgb_led.h"

//****************************************************************************
// Prototype(s):
//****************************************************************************

//****************************************************************************
// Definition(s):
//****************************************************************************

//Board Status
#define BOARD_STATUS_NA       	0
#define BOARD_STATUS_ALL_GOOD	1
#define BOARD_STATUS_ERROR      2
	
//Reassembles a uint32 from 4 bytes. b0 is the MSB.
#define BYTES_TO_UINT32(b0,b1,b2,b3)	(((uint32_t)b0 << 24) + \
										((uint32_t)b1 << 16) + \
										((uint32_t)b2 << 8) + \
										((uint32_t)b3))

//Reassembles a uint16 from 2 bytes. b0 is the MSB.
#define BYTES_TO_UINT16(b0,b1)			(((uint16_t)b0 << 8) + \
										((uint16_t)b1))
	
//Minimalist version of user.h:
//Enable/Disable sub-modules:

#define USE_I2C1			//Shared memory (EZI2C)
#define USE_I2T_LIMIT		//I2t current limit
	
#endif // MAIN_H_
