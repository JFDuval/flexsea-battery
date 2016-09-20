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
	[This file] fsm: state machine that defines the board's behavior
*****************************************************************************
	[Change log] (Convention: YYYY-MM-DD | author | comment)
	* 2016-09-20 | jfduval | Added proper header
	*
****************************************************************************/
	
#ifndef INC_FSM_H
#define INC_FSM_H

//****************************************************************************
// Include(s)
//****************************************************************************	
	
#include "main.h"

//****************************************************************************
// Structure(s)
//****************************************************************************	

struct flexsea_batt_s
{
	uint16 voltage;
	int16 current;
	int8 temperature;
	uint8 status_byte;
	
	uint16 voltage_filtered;	
};
	
//****************************************************************************
// Public Function Prototype(s):
//****************************************************************************

void battery_board_fsm(void);
uint8 board_status(void);
	
//****************************************************************************
// Definition(s):
//****************************************************************************

//Timing:
#define POWER_ON_TIME           1000
#define POWER_OFF_TIME          1000
#define FADE_ON_OFF_TIME        1000
#define FSM_STATE1_LED_PERIOD   250  
#define FSM_STATE1_LED_ONTIME   125  
#define LED_SCALING_FACTOR      4
//Note: Timing*Scaling should be < MAX_PWM (and as close to it as possible)

//LEDs:
#define MAX_PWM                 4095
#define MIN_PWM                 0
#define GREEN_LED_PWM(x)        PWM_1_WriteCompare(x)
#define RED_LED_PWM(x)          PWM_2_WriteCompare(x)
#define RED						0
#define GREEN					1
#define BLUE					2

//Button & output switch:
#define BUTTON_THRESHOLD        250
#define BUTTON_PRESSED_FILTER   75
#define OUTPUT_ON               1
#define OUTPUT_OFF              0

//Status byte flags:
#define STATUS_VOLT_LOW			1
#define STATUS_VOLT_HIGH		2
#define STATUS_CURRENT_WARN		4
#define STATUS_CURRENT_LIM		8

//FSM states:
#define FSM_STATE_STARTUP       0
#define FSM_STATE_POWER_ON      1
#define FSM_STATE_ON            2
#define FSM_STATE_POWER_OFF     3
#define FSM_STATE_OFF           4
	
//****************************************************************************
// Shared Variable(s):
//****************************************************************************	

extern struct flexsea_batt_s flexsea_batt;

#endif	//INC_FSM_H
	