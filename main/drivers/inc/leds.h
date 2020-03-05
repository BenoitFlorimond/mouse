/**
*******************************************************************************
* @file 	leds.h
* @author 	Benoit Florimond
* @date 	03/01/20
*******************************************************************************
*/

#ifndef __LEDS_H__
#define __LEDS_H__

/* ____________________________________________________________________________ */
/* Includes 																	*/
#include "system_def.h"

/* ____________________________________________________________________________ */
/* Defines 																		*/
#define LED_NO_PIN (0xFF)
#define LED_NO_HANDLE (0xFF)
/* ____________________________________________________________________________ */
/* Enum 																		*/

/* ____________________________________________________________________________ */
/* Struct																	 	*/

/* ____________________________________________________________________________ */
/* Public function prototypes 													*/
void vLED_Process(void* pvParameters);

uint8_t u8LED_RegisterLed(uint32_t rGpio, uint32_t gGpio, uint32_t bGpio);

void vLED_SetLedSolid(uint8_t ledHandle, uint32_t rgbColor, bool fade, uint32_t delayToFadeMs);

void vLED_SetLedBlinking(uint8_t ledHandle, uint32_t color, uint32_t periodMs, bool fade, uint32_t delayToFadeMs);

void vLED_SetLedOff(uint8_t ledHandle);

#endif //__LEDS_H__