/**
******************************************************************************
* @file 	buttonsManager.c
* @author 	Benoit Florimond
* @date 	09/23/19
******************************************************************************
*/
#include "buttonsManager.h"

void vBUTMNGR_Process(void* pvParameters)
{
    printf("Hello world from task!\n");

    for (int i = 10; i >= 0; i--) {
        printf("Mouse restarting in %d seconds...\n", i);
        vTaskDelay(1000 / portTICK_PERIOD_MS);
    }
    printf("Restarting now.\n");
    fflush(stdout);
    esp_restart();
}
