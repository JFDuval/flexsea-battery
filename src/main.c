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
	* 2016-12-16 | jfduval | Integrated main_fsm
****************************************************************************/

//****************************************************************************
// Include(s)
//****************************************************************************

#include "main.h"

//****************************************************************************
// Variable(s)
//****************************************************************************

//****************************************************************************
// Function(s)
//****************************************************************************

int main()
{	
	//Initialize and start peripherals:
    init_peripherals();
	
	//Test code:
	//test_vb_filter_blocking();
	//rgbLedRefresh_testcode_blocking();
	
	//Enable global interrupts
	CyGlobalIntEnable;
	
    while(1)
    {
		if(t1_new_value == 1)
		{
			//If the time share slot changed we run the timing FSM. 
			//'t1_new_value' updates at 10kHz, each slot at 1kHz.			
            
            t1_new_value = 0;            
			
			//Timing FSM:
			switch(t1_time_share)
			{
				case 0:                    
					main_fsm_case_0();	
					break;				
				case 1:       
					main_fsm_case_1();	
					break;				
				case 2:
					main_fsm_case_2();
					break;
				case 3:				
					main_fsm_case_3();					
					break;
				case 4:
					main_fsm_case_4();			
					break;				
				case 5:
					main_fsm_case_5();			
					break;					
				case 6:
					main_fsm_case_6();						
					break;				
				case 7:					
					main_fsm_case_7();	
					break;				
				case 8:
					main_fsm_case_8();					
					break;
				case 9:
					main_fsm_case_9();	
					break;				
				default:
					break;
			}
			
			//Increment value, limits to 0-9
        	t1_time_share++;
	        t1_time_share %= 10;
			
			//The code below is executed every 100us, after the previous slot. 
			//Keep it short! (<10us if possible)
			main_fsm_10kHz();         
		}
		else
		{
			//Asynchronous code goes here.
			main_fsm_asynchronous();			
		}
    }
}
