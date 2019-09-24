/**
******************************************************************************
* @file 	buttons.c
* @author 	Benoit Florimond
* @date 	09/23/19
******************************************************************************
*/

#include "buttons.h"
#include "driver/gpio.h"

static xQueueHandle _queueForButtons = NULL;

static void IRAM_ATTR vButtonsISR(void* arg);
static void vInitButtons(void);

static void IRAM_ATTR vButtonsISR(void* arg)
{
    uint32_t gpio_num = (uint32_t)arg;
    xQueueSendFromISR(_queueForButtons, &gpio_num, NULL);
}

void vBUT_Process(void* pvParameters)
{
    static uint32_t _buttonPin = 0;
    vInitButtons();

    for (;;) {
        if (xQueueReceive(_queueForButtons, &_buttonPin, portMAX_DELAY)) {
            printf("GPIO[%d] intr, val: %d\n", _buttonPin, gpio_get_level(_buttonPin));
        }
    }
}

static void vInitButtons(void)
{
    gpio_config_t gpioConfig = { 0 };

    _queueForButtons = xQueueCreate(10, sizeof(uint32_t));

    gpioConfig.pin_bit_mask = GPIO_SEL_25;
    gpioConfig.mode = GPIO_MODE_INPUT;
    gpioConfig.pull_down_en = GPIO_PULLDOWN_ENABLE;
    gpioConfig.pull_up_en = GPIO_PULLUP_DISABLE;
    gpioConfig.intr_type = GPIO_INTR_ANYEDGE;

    gpio_config(&gpioConfig);

    gpio_install_isr_service(0);

    gpio_isr_handler_add(GPIO_NUM_25, vButtonsISR, (void*)GPIO_NUM_25);
}
