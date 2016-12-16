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
	[This file] misc: when it doesn't belong in any another file, it ends 
	up here...
*****************************************************************************
	[Change log] (Convention: YYYY-MM-DD | author | comment)
	* 2016-09-20 | jfduval | Added proper header
	*
****************************************************************************/
	
#ifndef INC_MISC_H
#define INC_MISC_H

//****************************************************************************
// Include(s)
//****************************************************************************	
	
#include "main.h"

//****************************************************************************
// Structure(s)
//****************************************************************************	
	
extern volatile uint8 t1_time_share, t1_new_value;
	
//****************************************************************************
// Public Function Prototype(s):
//****************************************************************************

void init_peripherals(void);
uint8 timebase_1s(void);
uint8 timebase_100ms(void);
void uint32_to_bytes(uint32_t x, uint8_t *b0, uint8_t *b1, uint8_t *b2, uint8_t *b3);
void uint16_to_bytes(uint32_t x, uint8_t *b0, uint8_t *b1);

//****************************************************************************
// Definition(s):
//****************************************************************************

//LED:
#define LED_PERIOD_NORM			1500
#define LED_PERIOD_ERROR		200	
#define LED_PULSE_LEN			80	

#define LED_MODE_PWM            0    
#define LED_MODE_BRIEF_PULSE    1
	
#endif	//INC_MISC_H
	