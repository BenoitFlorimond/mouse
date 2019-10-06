/**
******************************************************************************
* @file 	buttonsManager.c
* @author 	Benoit Florimond
* @date 	09/23/19
******************************************************************************
*/
#include "buttonsManager.h"
#include "buttons.h"

#define BUT_MNGR_TAG ("BUT_MNGR")
#define BUTTON_GO_GPIO_NUM (GPIO_NUM_25)
#define BUTTON_FORWARD_GPIO_NUM (GPIO_NUM_25)
#define BUTTON_BACKWARD_GPIO_NUM (GPIO_NUM_25)
#define BUTTON_LEFT_GPIO_NUM (GPIO_NUM_25)
#define BUTTON_RIGHT_GPIO_NUM (GPIO_NUM_25)

static QueueHandle_t _buttonManagerQueue = NULL;

void vBUTMNGR_Process(void* pvParameters)
{
    buttonEvent_t buttonEvent = { 0 };

    _buttonManagerQueue = xQueueCreate(10, sizeof(buttonEvent_t));
    bBUT_RegisterButton(BUTTON_GO_GPIO_NUM, 31, _buttonManagerQueue);

    for (;;) {
        if (xQueueReceive(_buttonManagerQueue, &buttonEvent, portMAX_DELAY) == pdTRUE) {
            if (buttonEvent.triggerBitmap & BUTTON_TRIGGER_EDGE_PRESSED) {
                ESP_LOGI(BUT_MNGR_TAG, "Button %d just pressed", buttonEvent.gpio);
            }
            if (buttonEvent.triggerBitmap & BUTTON_TRIGGER_EDGE_RELEASED) {
                ESP_LOGI(BUT_MNGR_TAG, "Button %d just released", buttonEvent.gpio);
            }
            if (buttonEvent.triggerBitmap & BUTTON_TRIGGER_SHORT_PRESS) {
                ESP_LOGI(BUT_MNGR_TAG, "Button %d short press", buttonEvent.gpio);
            }
            if (buttonEvent.triggerBitmap & BUTTON_TRIGGER_LONG_PRESS) {
                ESP_LOGI(BUT_MNGR_TAG, "Button %d long press", buttonEvent.gpio);
            }
            if (buttonEvent.triggerBitmap & BUTTON_TRIGGER_VERY_LONG_PRESS) {
                ESP_LOGI(BUT_MNGR_TAG, "Button %d very long press", buttonEvent.gpio);
            }
        }
    }
}
