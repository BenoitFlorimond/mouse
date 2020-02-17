/**
*******************************************************************************
* @file 	mapping.h
* @author 	Benoit Florimond
* @date 	02/17/20
*******************************************************************************
*/

#ifndef __MAPPING_H__
#define __MAPPING_H__

/* ____________________________________________________________________________ */
/* Includes 																	*/
#include "driver/gpio.h"

/* ____________________________________________________________________________ */
/* Defines 																		*/

#define BUTTON_GO_GPIO_NUM (GPIO_NUM_25)
#define BUTTON_RESET_GPIO_NUM (GPIO_NUM_14)
#define BUTTON_FORWARD_GPIO_NUM (GPIO_NUM_27)
#define BUTTON_BACKWARD_GPIO_NUM (GPIO_NUM_33)
#define BUTTON_LEFT_GPIO_NUM (GPIO_NUM_26)
#define BUTTON_RIGHT_GPIO_NUM (GPIO_NUM_32)

#define SERVO_LEFT_GPIO_NUM (GPIO_NUM_12)
#define SERVO_RIGHT_GPIO_NUM (GPIO_NUM_13)

/* ____________________________________________________________________________ */
/* Enum 																		*/

/* ____________________________________________________________________________ */
/* Struct																	 	*/

/* ____________________________________________________________________________ */
/* Public function prototypes 													*/

#endif //__MAPPING_H__