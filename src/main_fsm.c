/****************************************************************************
	[Project] FlexSEA: Flexible & Scalable Electronics Architecture
	[Sub-project] 'flexsea-battery' Battery Protection & Monitoring
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
	[This file] main_fsm: Contains all the case() code for the main FSM
*****************************************************************************
	[Change log] (Convention: YYYY-MM-DD | author | comment)
	* 2016-12-15 | jfduval | Copied from Execute
	*
****************************************************************************/

//****************************************************************************
// Include(s)
//****************************************************************************

#include "main.h"
#include "main_fsm.h"

//****************************************************************************
// Variable(s)
//****************************************************************************

uint8 cmd_ready = 0;
uint8 i2c_flag = 0;

//****************************************************************************
// Private Function Prototype(s):
//****************************************************************************	

//****************************************************************************
// Public Function(s)
//****************************************************************************

//1kHz time slots:
//================

//Case 0: Update voltage and current
void main_fsm_case_0(void)
{
	read_vb_mv();
	read_ib_ma();			
}

//Case 1: Update temperature
void main_fsm_case_1(void)
{
	read_temp_celsius();			
}

//Case 2: Shared memory
void main_fsm_case_2(void)
{	
	//Update shared memory:
	update_ezi2c_mem();
}

//Case 3: 100ms timebase & I2t limits
void main_fsm_case_3(void)
{			
	#ifdef USE_I2T_LIMIT
	//Sample current (I2t limit):
	i2t_sample(flexsea_batt.current);
	#endif	//USE_I2T_LIMIT
			
	//100ms timebase:
	if(timebase_100ms())
	{
		#ifdef USE_I2T_LIMIT
		//Is the current in range?
		currentLimit = i2t_compute();
		#endif	//USE_I2T_LIMIT
	} 
}

//Case 4: User Interface
void main_fsm_case_4(void)
{
	//Update error flags:
	board_status();	
}

//Case 5: 
void main_fsm_case_5(void)
{
	//This state-machine defines the behaviour of the board
	battery_board_fsm();
}

//Case 6: 
void main_fsm_case_6(void)
{
	//UI RGB LED:	
	rgbLedRefreshFade();
}

//Case 7:
void main_fsm_case_7(void)
{

}

//Case 8: 
void main_fsm_case_8(void)
{

}

//Case 9: 1s timebase
void main_fsm_case_9(void)
{
	//1s timebase:
	if(timebase_1s())
	{
		//Insert code that needs to run every second here
		//...
	} 
}

//10kHz time slot:
//================

void main_fsm_10kHz(void)
{
	//RGB LED:
	rgbLedRefresh();
}

//Asynchronous time slots:
//========================

void main_fsm_asynchronous(void)
{
	//EZI2C Write complete
    if (0u != (I2C_1_EzI2CGetActivity() & I2C_1_EZI2C_STATUS_WRITE1))
    {
		//...
		i2c_flag = 1;
		i2c_flag = 0;	
		//ToDo: what do we do with this flag?
    }
}

//****************************************************************************
// Private Function(s)
//****************************************************************************
