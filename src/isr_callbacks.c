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
	[This file] isr_callbacks: ISR code placed here rather than in auto 
	generated files
*****************************************************************************
	[Change log] (Convention: YYYY-MM-DD | author | comment)
	* 2016-09-20 | jfduval | Added proper header
	*
****************************************************************************/

//****************************************************************************
// Include(s)
//****************************************************************************

#include "main.h"
#include "isr_callbacks.h"
#include "misc.h"

//****************************************************************************
// Variable(s)
//****************************************************************************


//****************************************************************************
// Public Function(s)
//****************************************************************************

//Timer 1 ISR:
CY_ISR(isr_t1_Interrupt_Callback)
{
	static uint16 cnt = 0;
	static uint16 timebase_100ms = 0;

	//Clear interrupt
	Timer_1_ReadStatusRegister();
	isr_t1_ClearPending();

	//Brief pulse every second if nothing is wrong:

    if(led_mode == LED_MODE_BRIEF_PULSE)
    {
    	if(cnt < LED_PULSE_LEN)
    		LED_G_Write(1);
    	else
    		LED_G_Write(0);
    }

	cnt++;
	if(cnt >= led_period)
		cnt = 0;

	//1ms timebase:
	flag_tb_1ms = 1;

	//100ms timebase:
	timebase_100ms++;
	if(timebase_100ms >= 100)
	{
		timebase_100ms = 0;
		flag_tb_100ms = 1;
	}
}

//Sequencing ADC ISR:
CY_ISR(ADC_SAR_Seq_1_ISR_Callback)
{
	uint32 intr_status;
	static uint8 ch = 0;
	
	//Read interrupt status register
	intr_status = ADC_SAR_Seq_1_SAR_INTR_REG;
	
	//Copy data
	for(ch = 0; ch <= MAX_ADC_CH; ch++)
	{
		adc_res[ch] = ADC_SAR_Seq_1_GetResult16(ch);
	}
	//ToDo: optimize by reading from register

	//Clear handled interrupt:
	//ADC_SAR_Seq_1_IRQ_ClearPending();
	ADC_SAR_Seq_1_SAR_INTR_REG = intr_status;
}

//****************************************************************************
// Private Function(s)
//****************************************************************************
