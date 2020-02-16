/**
*******************************************************************************
* @file 	servo.h
* @author 	Benoit Florimond
* @date 	10/09/19
*******************************************************************************
*/

#ifndef __SERVO_H__
#define __SERVO_H__

/* ____________________________________________________________________________ */
/* Includes 																	*/
#include "system_def.h"

/* ____________________________________________________________________________ */
/* Defines 																		*/

/* ____________________________________________________________________________ */
/* Enum 																		*/

/* ____________________________________________________________________________ */
/* Struct																	 	*/

/* ____________________________________________________________________________ */
/* Public function prototypes 													*/
void vSERVO_Process(void* pvParameters);

bool bSERVO_RegisterServo(uint32_t gpio, float minPulseMs, float maxPulseMs);

bool bSERVO_SetOrder(uint32_t gpio, float speed, bool forward);

#endif //__SERVO_H__