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
	[This file] sensors: sensor data acquisition, conversion and filtering
*****************************************************************************
	[Change log] (Convention: YYYY-MM-DD | author | comment)
	* 2016-09-20 | jfduval | Added proper header
	*
****************************************************************************/
	
#ifndef INC_SENSORS_H
#define INC_SENSORS_H

//****************************************************************************
// Include(s)
//****************************************************************************	
	
#include "main.h"

//****************************************************************************
// Structure(s)
//****************************************************************************	

	
//****************************************************************************
// Public Function Prototype(s):
//****************************************************************************

uint16 read_vb_mv(void);
uint16 filter_vb_mv(uint16 newVal);
int16 read_ib_ma(void);
int8 read_temp_celsius(void);
void update_ezi2c_mem(void);

void test_vb_filter_blocking(void);

//****************************************************************************
// Definition(s):
//****************************************************************************

//Sequencing ADC:	
#define ADC_CHANNELS			4
#define MAX_ADC_CH 				(ADC_CHANNELS-1)
#define ADC_VMEAS				0
#define ADC_IMEAS				1
#define ADC_BUTTON              2
#define ADC_TEMP				3

//Limits:
#define VOLTAGE_MIN				15000
#define VOLTAGE_MAX				52000
//See I2t-current-limit for the current limit
#define TEMP_MAX				85
//Note: should be modified for your application!

//Filters:
#define VB_SAMPLES				32
#define VB_SHIFT				5

//****************************************************************************
// Shared Variable(s):
//****************************************************************************	
	
extern int16 adc_res[ADC_CHANNELS];
extern uint8 currentLimit;
	
#endif	//INC_SENSORS_H
	