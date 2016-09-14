//****************************************************************************
// MIT Media Lab - Biomechatronics
// Jean-Francois (Jeff) Duval
// jfduval@mit.edu
// 03/2015
//****************************************************************************
// sensors: sensor data acquisition, conversion and filtering
//****************************************************************************

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
	filter_vb_mv(flexsea_batt.voltage);
	
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
