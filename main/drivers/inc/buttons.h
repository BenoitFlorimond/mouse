/**
******************************************************************************
* @file 	buttons.h
* @author 	Benoit Florimond
* @date 	09/23/19
******************************************************************************
*/

#ifndef __BUTTONS_H__
#define __BUTTONS_H__

#include "system_def.h"

#define BUTTON_TRIGGER_NONE (0)
#define BUTTON_TRIGGER_EDGE_PRESSED (BIT(0))
#define BUTTON_TRIGGER_EDGE_RELEASED (BIT(1))
#define BUTTON_TRIGGER_SHORT_PRESS (BIT(2))
#define BUTTON_TRIGGER_LONG_PRESS (BIT(3))
#define BUTTON_TRIGGER_VERY_LONG_PRESS (BIT(4))

typedef struct {
    uint32_t gpio;
    uint32_t triggerBitmap;
} buttonEvent_t;

void vBUT_Process(void* pvParameters);

bool bBUT_RegisterButton(uint32_t gpio, uint32_t triggerBitmap, QueueHandle_t eventQueue);

#endif //__BUTTONS_H__