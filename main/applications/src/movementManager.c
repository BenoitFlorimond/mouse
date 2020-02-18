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

#define SPEED_FORWARD (50.0F)
#define SPEED_BACKWARD (50.0F)
#define SPEED_ROTATION (30.0F)
#define SPEED_STOP (0.0F)

#define FORWARD_DELAY_TICKS (pdMS_TO_TICKS(1000U))
#define BACKWARD_DELAY_TICKS (pdMS_TO_TICKS(1000U))
#define ROTATION_DELAY_TICKS (pdMS_TO_TICKS(1000U))

/* ____________________________________________________________________________ */
/* Enum  																		*/

/* ____________________________________________________________________________ */
/* Struct																		*/

/* ____________________________________________________________________________ */
/* Static prototypes 															*/
static void _endMovementTimerCallback(void* timer);

/* ____________________________________________________________________________ */
/* Static variables 															*/
static QueueHandle_t _queueForMovement = NULL;
static TimerHandle_t _timerForMovement = NULL;

/* ____________________________________________________________________________ */
/* ISR handlers 																*/

/* ____________________________________________________________________________ */
/* Public functions 															*/
void vMVT_Process(void* pvParameters)
{
    movementType_e movementEvent = { 0 };

    _queueForMovement = xQueueCreate(5U, sizeof(movementEvent));
    _timerForMovement = xTimerCreate("Timer to end movement", FORWARD_DELAY_TICKS, pdFALSE, NULL, _endMovementTimerCallback);

    bSERVO_RegisterServo(SERVO_LEFT_GPIO_NUM, PULSE_WIDTH_MIN_MS, PULSE_WIDTH_MAX_MS);
    bSERVO_RegisterServo(SERVO_RIGHT_GPIO_NUM, PULSE_WIDTH_MIN_MS, PULSE_WIDTH_MAX_MS);

    for (;;) {
        xQueueReceive(_queueForMovement, &movementEvent, portMAX_DELAY);

        switch (movementEvent) {
        case MOVEMENT_STOP:
            bSERVO_SetOrder(SERVO_LEFT_GPIO_NUM, SPEED_STOP, true);
            bSERVO_SetOrder(SERVO_RIGHT_GPIO_NUM, SPEED_STOP, true);
            xTimerStop(_timerForMovement, TIMER_API_DEFAULT_TIMEOUT);
            break;
        case MOVEMENT_FORWARD:
            bSERVO_SetOrder(SERVO_LEFT_GPIO_NUM, SPEED_FORWARD, true);
            bSERVO_SetOrder(SERVO_RIGHT_GPIO_NUM, SPEED_FORWARD, true);
            xTimerChangePeriod(_timerForMovement, FORWARD_DELAY_TICKS, TIMER_API_DEFAULT_TIMEOUT);
            xTimerReset(_timerForMovement, TIMER_API_DEFAULT_TIMEOUT);
            break;
        case MOVEMENT_BACKWARD:
            bSERVO_SetOrder(SERVO_LEFT_GPIO_NUM, SPEED_BACKWARD, false);
            bSERVO_SetOrder(SERVO_RIGHT_GPIO_NUM, SPEED_BACKWARD, false);
            xTimerChangePeriod(_timerForMovement, BACKWARD_DELAY_TICKS, TIMER_API_DEFAULT_TIMEOUT);
            xTimerReset(_timerForMovement, TIMER_API_DEFAULT_TIMEOUT);
            break;
        case MOVEMENT_ROTATION_LEFT:
            bSERVO_SetOrder(SERVO_LEFT_GPIO_NUM, SPEED_ROTATION, false);
            bSERVO_SetOrder(SERVO_RIGHT_GPIO_NUM, SPEED_ROTATION, true);
            xTimerChangePeriod(_timerForMovement, ROTATION_DELAY_TICKS, TIMER_API_DEFAULT_TIMEOUT);
            xTimerReset(_timerForMovement, TIMER_API_DEFAULT_TIMEOUT);
            break;
        case MOVEMENT_ROTATION_RIGHT:
            bSERVO_SetOrder(SERVO_LEFT_GPIO_NUM, SPEED_ROTATION, true);
            bSERVO_SetOrder(SERVO_RIGHT_GPIO_NUM, SPEED_ROTATION, false);
            xTimerChangePeriod(_timerForMovement, ROTATION_DELAY_TICKS, TIMER_API_DEFAULT_TIMEOUT);
            xTimerReset(_timerForMovement, TIMER_API_DEFAULT_TIMEOUT);
            break;
        default:
            break;
        }
    }
}

void vMVT_Move(movementType_e movement)
{
    xQueueSend(_queueForMovement, &movement, WRITE_IN_QUEUE_DEFAULT_TIMEOUT);
}

void vMVT_EmergencyStop(void)
{
    movementType_e movement = MOVEMENT_STOP;

    xQueueReset(_queueForMovement);
    xQueueSendToFront(_queueForMovement, &movement, 0U);
}

/* ____________________________________________________________________________ */
/* Static functions 															*/

static void _endMovementTimerCallback(void* timer)
{
    movementType_e movement = MOVEMENT_STOP;

    xQueueSendToFront(_queueForMovement, &movement, 0U);
}
