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
	
#ifndef INC_ISRCALLBACKS_H
#define INC_ISRCALLBACKS_H
	
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
	
CY_ISR_PROTO(isr_t1_Interrupt_Callback);
CY_ISR_PROTO(ADC_SAR_Seq_1_ISR_Callback);

//****************************************************************************
// Definition(s):
//****************************************************************************

//****************************************************************************
// Shared Variable(s):
//****************************************************************************	

#endif //INC_ISRCALLBACKS_H  
