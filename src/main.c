//****************************************************************************
// MIT Media Lab - Biomechatronics
// Jean-Francois (Jeff) Duval
// jfduval@mit.edu
// 03/2015
//****************************************************************************
// main: FlexSEA-Execute Safety-CoP
//****************************************************************************

//****************************************************************************
// Include(s)
//****************************************************************************

#include "main.h"

//****************************************************************************
// Variable(s)
//****************************************************************************

volatile int16 testCurrent = 10500;

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
