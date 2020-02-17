/**
*******************************************************************************
* @file 	movementManager.c
* @author 	Benoit Florimond
* @date 	02/17/20
*******************************************************************************
*/

/* ____________________________________________________________________________ */
/* Includes  																	*/
#include "movementManager.h"
#include "mapping.h"
#include "servo.h"

/* ____________________________________________________________________________ */
/* Defines  																	*/
#define SER_MNGR_TAG ("SER_MNGR")
#define PULSE_WIDTH_MIN_MS (1.0F)
#define PULSE_WIDTH_MAX_MS (2.0F)

/* ____________________________________________________________________________ */
/* Enum  																		*/
typedef enum {
    STOP = 0,
    FORWARD,
    BACKWARD,
    ROTATION_LEFT,
    ROTATION_RIGHT,
} movementType_e;

/* ____________________________________________________________________________ */
/* Struct																		*/

/* ____________________________________________________________________________ */
/* Static prototypes 															*/

/* ____________________________________________________________________________ */
/* Static variables 															*/
QueueHandle_t _queueForMovement = NULL;

/* ____________________________________________________________________________ */
/* ISR handlers 																*/

/* ____________________________________________________________________________ */
/* Public functions 															*/
void vMVT_Process(void* pvParameters)
{
    bSERVO_RegisterServo(SERVO_LEFT_GPIO_NUM, PULSE_WIDTH_MIN_MS, PULSE_WIDTH_MAX_MS);
    bSERVO_RegisterServo(SERVO_RIGHT_GPIO_NUM, PULSE_WIDTH_MIN_MS, PULSE_WIDTH_MAX_MS);

    // _queueForMovement = xQueueCreatse();

    for (;;) {
    }
}
/* ____________________________________________________________________________ */
/* Static functions 															*/
