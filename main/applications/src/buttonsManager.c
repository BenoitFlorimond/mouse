/**
******************************************************************************
* @file 	buttonsManager.c
* @author 	Benoit Florimond
* @date 	09/23/19
******************************************************************************
*/
#include "buttonsManager.h"
#include "buttons.h"
#include "mapping.h"

#define BUT_MNGR_TAG ("BUT_MNGR")

static QueueHandle_t _buttonManagerQueue = NULL;

void vBUTMNGR_Process(void* pvParameters)
{
    buttonEvent_t buttonEvent = { 0 };

    _buttonManagerQueue = xQueueCreate(10, sizeof(buttonEvent_t));
    bBUT_RegisterButton(BUTTON_GO_GPIO_NUM, BUTTON_TRIGGER_EDGE_PRESSED, _buttonManagerQueue);
    bBUT_RegisterButton(BUTTON_RESET_GPIO_NUM, BUTTON_TRIGGER_EDGE_PRESSED, _buttonManagerQueue);
    bBUT_RegisterButton(BUTTON_FORWARD_GPIO_NUM, BUTTON_TRIGGER_EDGE_PRESSED, _buttonManagerQueue);
    bBUT_RegisterButton(BUTTON_BACKWARD_GPIO_NUM, BUTTON_TRIGGER_EDGE_PRESSED, _buttonManagerQueue);
    bBUT_RegisterButton(BUTTON_LEFT_GPIO_NUM, BUTTON_TRIGGER_EDGE_PRESSED, _buttonManagerQueue);
    bBUT_RegisterButton(BUTTON_RIGHT_GPIO_NUM, BUTTON_TRIGGER_EDGE_PRESSED, _buttonManagerQueue);

    for (;;) {
        if (xQueueReceive(_buttonManagerQueue, &buttonEvent, portMAX_DELAY) == pdTRUE) {
            ESP_LOGI(BUT_MNGR_TAG, "Button %d trigged an event %d", buttonEvent.gpio, buttonEvent.triggerBitmap);
            switch (buttonEvent.gpio) {
            case BUTTON_GO_GPIO_NUM:
                break;
            case BUTTON_RESET_GPIO_NUM:
                break;
            case BUTTON_FORWARD_GPIO_NUM:
                break;
            case BUTTON_BACKWARD_GPIO_NUM:
                break;
            case BUTTON_LEFT_GPIO_NUM:
                break;
            case BUTTON_RIGHT_GPIO_NUM:
                break;
            default:
                break;
            }
        }
    }
}
