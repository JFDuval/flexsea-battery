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

//Note: It's a sign that code is misplaced when this file gets long.


//****************************************************************************
// Include(s)
//****************************************************************************

#include "main.h"
#include "misc.h"

//****************************************************************************
// Variable(s)
//****************************************************************************

volatile uint8 t1_time_share = 0, t1_new_value = 0;
uint8_t newDataLED = 0;

//****************************************************************************
// Public Function(s)
//****************************************************************************

void init_peripherals(void)
{
    //Timer 1 (1ms timebase):
	Timer_1_Start();
	isr_t1_StartEx(isr_t1_Interrupt_Callback);
    
	//Sequencing ADC:
	ADC_SAR_Seq_1_Start();
	ADC_SAR_Seq_1_IRQ_StartEx(ADC_SAR_Seq_1_ISR_Callback);
	ADC_SAR_Seq_1_StartConvert();	
	 
	//IDAC:
	IDAC_1_Start();
    Opamp_0_Start();
    Opamp_1_Start();

    //LEDs start at 0:
	LED_R_Write(LED_OFF);
	LED_G_Write(LED_OFF);
    LED_B_Write(LED_OFF);
	rgbLedSet(0,0,0);	
    
	#ifdef USE_I2C1
	//EZI2C:	
	I2C_1_EzI2CSetBuffer1(EZI2C_BUF_SIZE, EZI2C_WBUF_SIZE, ezI2Cbuf);
	init_ezI2Cbuf();
	I2C_1_Start();
	#endif
}

//From 1 uint32 to 4 uint8
void uint32_to_bytes(uint32_t x, uint8_t *b0, uint8_t *b1, uint8_t *b2, uint8_t *b3)
{
	*b0 = (uint8_t) ((x >> 24) & 0xFF);
	*b1 = (uint8_t) ((x >> 16) & 0xFF);
	*b2 = (uint8_t) ((x >> 8) & 0xFF);
	*b3 = (uint8_t) (x & 0xFF);
}

//From 1 uint16 to 2 uint8
void uint16_to_bytes(uint32_t x, uint8_t *b0, uint8_t *b1)
{
	*b0 = (uint8_t) ((x >> 8) & 0xFF);
	*b1 = (uint8_t) (x & 0xFF);
}

//Call this function in the 1kHz FSM. It will return 1 every second.
uint8 timebase_1s(void)
{
	static uint16 time = 0;
	
	time++;
	if(time >= 999)
	{
		time = 0;
		return 1;
	}
	
	return 0;
}

//Call this function in the 1kHz FSM. It will return 1 every 100ms.
uint8 timebase_100ms(void)
{
	static uint16 time = 0;
	
	time++;
	if(time >= 99)
	{
		time = 0;
		return 1;
	}
	
	return 0;
}
