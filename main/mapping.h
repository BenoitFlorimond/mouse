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

#define BUTTON_GO_GPIO_NUM (GPIO_NUM_35)
#define BUTTON_RESET_GPIO_NUM (GPIO_NUM_34)
#define BUTTON_BACK_GPIO_NUM (GPIO_NUM_10)
#define BUTTON_FORWARD_GPIO_NUM (GPIO_NUM_27)
#define BUTTON_BACKWARD_GPIO_NUM (GPIO_NUM_33)
#define BUTTON_LEFT_GPIO_NUM (GPIO_NUM_26)
#define BUTTON_RIGHT_GPIO_NUM (GPIO_NUM_32)

#define SERVO_LEFT_GPIO_NUM (GPIO_NUM_23)
#define SERVO_RIGHT_GPIO_NUM (GPIO_NUM_25)

#define LED_LEFT_R_GPIO_NUM (GPIO_NUM_16)
#define LED_LEFT_G_GPIO_NUM (GPIO_NUM_17)
#define LED_LEFT_B_GPIO_NUM (GPIO_NUM_18)
#define LED_RIGHT_R_GPIO_NUM (GPIO_NUM_19)
#define LED_RIGHT_G_GPIO_NUM (GPIO_NUM_21)
#define LED_RIGHT_B_GPIO_NUM (GPIO_NUM_22)

#define BUZZER_GPIO_NUM (GPIO_NUM_4)
#define CHARGE_STATUS_GPIO_NUM (GPIO_NUM_5)

/* ____________________________________________________________________________ */
/* Enum 																		*/

/* ____________________________________________________________________________ */
/* Struct																	 	*/

/* ____________________________________________________________________________ */
/* Public function prototypes 													*/

#endif //__MAPPING_H__