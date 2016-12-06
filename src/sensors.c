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

//****************************************************************************
// Include(s)
//****************************************************************************

#include "main.h"
#include "sensors.h"

//****************************************************************************
// Variable(s)
//****************************************************************************

int16 adc_res[ADC_CHANNELS];
int16 adc_zero = 1024;	
uint8 currentLimit = RET_I2T_NORMAL;

//****************************************************************************
// Function(s)
//****************************************************************************

//Battery voltage (in mV) - instantaneous
uint16 read_vb_mv(void)
{
	uint32 tmp1 = 0;
	double tmp2 = 0;
	
	tmp1 = 3300 * adc_res[ADC_VMEAS];
	tmp2 = 11.729 * ((tmp1 / 2048) + 958.8);
	
	flexsea_batt.voltage = (uint16)tmp2;
	
	//Get filtered version:
	//filter_vb_mv(flexsea_batt.voltage);	//Moving average
	//Butterworth 1st order 2.5Hz:
	flexsea_batt.voltage_filtered = filter_vb_mv(flexsea_batt.voltage);
	
	return flexsea_batt.voltage;
}

//Moving average filter.
uint16 filter_vb_mv(uint16 newVal)
{
	int i = 0, sum = 0;
	static int idx = 0, iter = 0;
	static uint16 vbBuffer[VB_SAMPLES];
	uint16 retVal = 0;
	
	//We do not want to get false values before the buffer is filled:
	if(iter < VB_SAMPLES)
	{
		//First time?
		if(iter == 0)
		{
			//Initialize array:
			for(idx = 0; idx < VB_SAMPLES; idx++)
			{
				vbBuffer[idx] = 0;
			}
		}
		
		vbBuffer[iter] = newVal;
		retVal = newVal; //Returns latest val (no filter)
		iter++;		
	}
	else
	{
		//Normal operation - buffer is full:
		
		//Shift
		for(i = 1; i < VB_SAMPLES; i++)
		{
			vbBuffer[i-1] = vbBuffer[i];
			sum += vbBuffer[i];
		}
		vbBuffer[VB_SAMPLES-1] = newVal;
		
		//Avg:
		sum += newVal;
		retVal = sum >> VB_SHIFT;	//Filtered value
	}
	
	flexsea_batt.voltage_filtered = retVal;
	return retVal;
}

int16 read_ib_ma(void)
{
	double tmp1 = 0;
	
	tmp1 = (adc_res[ADC_IMEAS] - adc_zero) * 36.62;
	
	flexsea_batt.current = (int16)tmp1;
	return flexsea_batt.current;
}

//ToDo: confirm measurement, seems wrong (62C on my desk...)
int8 read_temp_celsius(void)
{
	flexsea_batt.temperature = (int8) DieTemp_1_CountsTo_Celsius((int32)adc_res[ADC_TEMP]);
	return flexsea_batt.temperature;
}

//Update shared memory:
void update_ezi2c_mem(void)
{	
	uint8 tmp0 = 0, tmp1 = 0;
	
	ezI2Cbuf[MEM_R_STATUS1] = flexsea_batt.status_byte;
	ezI2Cbuf[MEM_R_STATUS2] = 0;
	uint16_to_bytes((uint16)flexsea_batt.voltage, &tmp0, &tmp1);
	ezI2Cbuf[MEM_R_VOLT_MSB] = tmp0;
	ezI2Cbuf[MEM_R_VOLT_LSB] = tmp1;
	uint16_to_bytes((uint16)flexsea_batt.current, &tmp0, &tmp1);
	ezI2Cbuf[MEM_R_CURRENT_MSB] = tmp0;
	ezI2Cbuf[MEM_R_CURRENT_LSB] = tmp1;
	ezI2Cbuf[MEM_R_TEMP] = flexsea_batt.temperature;
}

//Takes the latest measurement, and returns the latest output
int lpfVoltage(int newVal)
{
	static int input[2] = {0,0};
	static int output[2] = {0,0};
	
	//Shift arrays:
	input[1] = input[0];
	output[1] = output[0];
	input[0] = newVal;
	
	//Calculate latest value:
	filt_array(input, output);
	
	//Return value:
	return (output[0] >> LPF_PARAM_3);
}

//Butterworth LPF:
void filt_array(int *raw,int *filt)
{
    filt[0] = (int)(((LPF_PARAM_1*(raw[0]+raw[1]))
              +LPF_PARAM_2*filt[1]+LPF_PARAM_4)>>LPF_PARAM_3);
}

//****************************************************************************
// Test code
//****************************************************************************

//Use this function to test the filter's behaviour and properties
//Moving average only
#define TEST_DATA_LEN	32
#define DEFAULT_VAL		20000	//mV
#define MOD_FACTOR		30
void test_vb_filter_blocking(void)
{
	uint16 loopCounter = 0, index = 0, actualVal = 0;
	//Latch board:
	SW_PSOC_Write(OUTPUT_ON);
	RED_LED_PWM(MAX_PWM);
	
	//Test data:
	uint16 testData[TEST_DATA_LEN];
	int i = 0;
	uint16_t mod = 0;
	for(i = 0; i < TEST_DATA_LEN; i++)
	{
		if(i < TEST_DATA_LEN)
			mod = MOD_FACTOR*i;
		else
			mod = MOD_FACTOR*(TEST_DATA_LEN-i);
			
		testData[i] = DEFAULT_VAL + mod;
	}
	
	//Feed it to the function under test:
	while(1)
	{
		actualVal = testData[index];
		filter_vb_mv(actualVal);
		index++;
		index %= TEST_DATA_LEN;
		loopCounter++;
	}	
}
