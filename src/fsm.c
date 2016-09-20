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
	[This file] fsm: state machine that defines the board's behavior
*****************************************************************************
	[Change log] (Convention: YYYY-MM-DD | author | comment)
	* 2016-09-20 | jfduval | Added proper header
	*
****************************************************************************/

//****************************************************************************
// Include(s)
//****************************************************************************

#include "main.h"
#include "fsm.h"

//****************************************************************************
// Variable(s)
//****************************************************************************

uint8 fsm_state = 0;
uint32 fsm_counter = 0;
uint8 fade_on_off = 0;
uint16 button_off_counter = 0;
uint8 status_ok = 0;
struct flexsea_batt_s flexsea_batt;
	
//****************************************************************************
// Private Function Prototype(s):
//****************************************************************************  

static uint8 button_pressed(void);
static void quick_led_flash(uint8 color);
static void fsm_transition_to_ON(void);
static void fsm_transition_to_POWER_OFF(void);

//****************************************************************************
// Public Function(s)
//****************************************************************************

//This state-machine defines the behaviour of the board
void battery_board_fsm(void)
{	
    switch(fsm_state)
    {
        //Startup - board just received power (via button push)
        //1 sec ramp-up, fade-on Green LED. Not latching yet.
        case FSM_STATE_STARTUP:
            fsm_counter++;
            if(fsm_counter >= POWER_ON_TIME)
            {
                fsm_counter = 0;
                fsm_state = FSM_STATE_POWER_ON;
            }
            GREEN_LED_PWM(fsm_counter * 4);
            break;
        
        //User still pressing? Display status.
        case FSM_STATE_POWER_ON:
            if(button_pressed())
            {
                //What's the board status?
                if(board_status() == BOARD_STATUS_ALL_GOOD)
                {
                    //Quick Green flash
					quick_led_flash(GREEN);
					status_ok = 1;
                }
                else
                {
                    //Quick Red flash
                    quick_led_flash(RED); 
					status_ok = 0;
                }
            }
            else
            {
                //User released the button
				if(status_ok)
				{
					//Status was good, turn ON
                	fsm_transition_to_ON();
				}
				else
				{
					//Problem detected, turn OFF
					fsm_state = FSM_STATE_OFF;
				}				
            }
            break;
            
        //Normal mode - fading Green LED
        case FSM_STATE_ON:
            
            //Latch output:
            SW_PSOC_Write(OUTPUT_ON);
            
            //Green LED fade on & off:
            if(fade_on_off == 0)
            {
                //Fade ON
                fsm_counter++;
                if(fsm_counter >= FADE_ON_OFF_TIME)
                {
                    //fsm_counter = 0;
                    fade_on_off = 1;
                }
            }
            else
            {
                //Fade OFF
                fsm_counter--;
                if(fsm_counter <= 0)
                {
                    //fsm_counter = 0;
                    fade_on_off = 0;
                }
            }
            GREEN_LED_PWM(fsm_counter * 4); 
            
            //Is the user pressing on the button?
            if(button_pressed())
            {
                button_off_counter++;
            }
            else
            {
                if(button_off_counter > 0)
                {
                    button_off_counter--;
                }
            }
            //Very short filter to avoid unecessary transition:
            if(button_off_counter >= BUTTON_PRESSED_FILTER)
            {
                //Move to next state:
				fsm_transition_to_POWER_OFF();
            }
			
			//Safety features:
			if(flexsea_batt.status_byte)
			{
				//Any flag will trigger a power OFF
				fsm_state = FSM_STATE_OFF;
			}
			//ToDo: add filters! This is quick & dirty code.
            
            break;
        
        //Board was ON, user pressing on button (turning it off)
        case FSM_STATE_POWER_OFF:
            
            //Is the user pressing on the button?
            if(button_pressed())
            {
                button_off_counter++;
            }
            else
            {
                if(button_off_counter > 0)
                {
                    button_off_counter--;
                }
            }
            
            //We detect a release by a difference between fsm_counter & button_off_counter
            if(button_off_counter < (fsm_counter - (fsm_counter >> 3)))
            {
                //Significant difference, go back to normal state
                fsm_transition_to_ON();
            }
            
            fsm_counter++;
            if(fsm_counter >= POWER_OFF_TIME)
            {
                fsm_counter = 0;
                fsm_state = FSM_STATE_OFF;
            }
            RED_LED_PWM(fsm_counter * 4);
            
            break;
            
        //User turned the board OFF    
        case FSM_STATE_OFF:   

            //Turn off output:
            SW_PSOC_Write(OUTPUT_OFF);
            
            //And now we wait for a peaceful death...
            
            break;
            
        default:
            LED_B_Write(1);     //Should not happen
            break;
    }
}

//Returns the board status
uint8 board_status(void)
{
	uint8 error = 0, status = BOARD_STATUS_NA;
	
    //Voltage in range?
	if(flexsea_batt.voltage_filtered <= VOLTAGE_MIN)
	{
		error++;
		flexsea_batt.status_byte |= STATUS_VOLT_LOW;
	}
	else
	{
		flexsea_batt.status_byte &= (~STATUS_VOLT_LOW);
	}
	
	if(flexsea_batt.voltage_filtered >= VOLTAGE_MAX)
	{
		error++;
		flexsea_batt.status_byte |= STATUS_VOLT_HIGH;
	}
	else
	{
		flexsea_batt.status_byte &= (~STATUS_VOLT_HIGH);
	}
	
    //Current in range? Warning zone
	if(currentLimit == RET_I2T_WARNING)
	{
		//error++;	//No error for a warning, but we should notify the user
		flexsea_batt.status_byte |= STATUS_CURRENT_WARN;
	}
	else
	{
		flexsea_batt.status_byte &= (~STATUS_CURRENT_WARN);
	}
	
	//Current in range? Over the limit
	if(currentLimit == RET_I2T_LIMIT)
	{
		error++;
		flexsea_batt.status_byte |= STATUS_CURRENT_LIM;
	}
	else
	{
		flexsea_batt.status_byte &= (~STATUS_CURRENT_LIM);
	}
	
	//More checks...
	//...
	
	//Return status:
	if(!error)
	{
		status = BOARD_STATUS_ALL_GOOD;
	}
	else
	{
		status = BOARD_STATUS_ERROR;
	}
    
    return status;
}

//****************************************************************************
// Private Function(s)
//****************************************************************************

//Is the pushbutton pressed?
static uint8 button_pressed(void)
{
    if(adc_res[ADC_BUTTON] <= BUTTON_THRESHOLD)
    {
        return 1;
    }
    else
    {
        return 0;
    }
    
    return 0;
}

//Quick LED flash
static void quick_led_flash(uint8 color)
{
	if(color == RED)
	{
 		GREEN_LED_PWM(MIN_PWM);    //Green OFF
        fsm_counter++;
        if(fsm_counter < FSM_STATE1_LED_ONTIME)
        {
            RED_LED_PWM(MAX_PWM);
        }
        else if((fsm_counter >= FSM_STATE1_LED_ONTIME) && (fsm_counter < FSM_STATE1_LED_PERIOD))
        {
            RED_LED_PWM(MIN_PWM);
        }
        else
        {
            fsm_counter = 0;
        }  	
	}
	else if(color == GREEN)    
	{
	    RED_LED_PWM(MIN_PWM);    //Red OFF
	    fsm_counter++;
	    if(fsm_counter < FSM_STATE1_LED_ONTIME)
	    {
	        GREEN_LED_PWM(MAX_PWM);
	    }
	    else if((fsm_counter >= FSM_STATE1_LED_ONTIME) && (fsm_counter < FSM_STATE1_LED_PERIOD))
	    {
	        GREEN_LED_PWM(MIN_PWM);
	    }
	    else
	    {
	        fsm_counter = 0;
	    }
	}	
}

//State transitions to ON
static void fsm_transition_to_ON(void)
{
    fsm_counter = 0;
    fsm_state = FSM_STATE_ON;
    fade_on_off = 0;
    //By default, LEDs OFF:
    PWM_1_WriteCompare(MIN_PWM);
    PWM_2_WriteCompare(MIN_PWM);
    LED_B_Write(0);
    //Button off counter starts at 0:
    button_off_counter = 0;    
}

//State transitions to POWER_OFF
static void fsm_transition_to_POWER_OFF(void)
{
	fsm_counter = 0;
    button_off_counter = 0;
    fsm_state = FSM_STATE_POWER_OFF;   
    GREEN_LED_PWM(MIN_PWM);  
}