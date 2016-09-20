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

//****************************************************************************
// Include(s)
//****************************************************************************

#include "main.h"

//****************************************************************************
// Variable(s)
//****************************************************************************

//volatile int16 testCurrent = 10500;

//****************************************************************************
// Function(s)
//****************************************************************************

int main()
{
	uint8 i2c_flag = 0;
	
	//Initialize and start peripherals:
    init_peripherals();
	
	//Enable global interrupts
	CyGlobalIntEnable;
	
    while(1)
    {
        //Every millisecond:		
		if(flag_tb_1ms)
		{
			flag_tb_1ms = 0;
			
			//Read sensors & convert data:
			read_vb_mv();
			read_ib_ma();
			read_temp_celsius();
			
			//Update shared memory:
			update_ezi2c_mem();
			
			//Sample current (I2t limit):
			i2t_sample(flexsea_batt.current);
			//i2t_sample(testCurrent);
			
			//Update error flags:
			board_status();
			
			//This state-machine defines the behaviour of the board
			battery_board_fsm();
		}
		
		//Every 100ms:
		if(flag_tb_100ms)
		{
			flag_tb_100ms = 0;
			
			//Is the current in range?
			currentLimit = i2t_compute();
		}
		      
		//EZI2C Write complete
        if (0u != (I2C_1_EzI2CGetActivity() & I2C_1_EZI2C_STATUS_WRITE1))
        {
			//...
			i2c_flag = 1;
			i2c_flag = 0;	
        }
    }
}
