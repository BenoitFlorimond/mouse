/* Hello World Example

   This example code is in the Public Domain (or CC0 licensed, at your option.)

   Unless required by applicable law or agreed to in writing, this
   software is distributed on an "AS IS" BASIS, WITHOUT WARRANTIES OR
   CONDITIONS OF ANY KIND, either express or implied.
*/
#include "buttons.h"
#include "buttonsManager.h"
#include "esp_spi_flash.h"
#include "leds.h"
#include "movementManager.h"
#include "sequenceManager.h"
#include "servo.h"

void app_main()
{
    /* Drivers tasks creation */
    xTaskCreate(vBUT_Process, "Driver buttons", 2048U, NULL, 7U, NULL);
    xTaskCreate(vSERVO_Process, "Driver servos", 2048U, NULL, 6U, NULL);
    xTaskCreate(vLED_Process, "Driver LEDs", 2048U, NULL, 5U, NULL);

    /* Applications tasks creation */
    xTaskCreate(vBUTMNGR_Process, "Buttons manager", 2048U, NULL, 3U, NULL);
    xTaskCreate(vMVT_Process, "Movement manager", 2048U, NULL, 2U, NULL);
    xTaskCreate(vSEQMNGR_Process, "Sequence manager", 2048U, NULL, 1U, NULL);
}
