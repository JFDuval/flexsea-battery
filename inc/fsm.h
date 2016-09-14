//****************************************************************************
// Dephy, Inc.
// Jean-Francois (JF) Duval
// jfduval@dephy.com
// 07/2016
//****************************************************************************
// sensors: sensor data acquisition, conversion and filtering
//****************************************************************************
	
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
	