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
typedef struct {
    movementType_e movement;
    void (*endCallback)(void);
} movementEvent_t;

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
    static void (*_endCallbackToCall)(void) = NULL;
    movementEvent_t movementEvent = { 0 };

    _queueForMovement = xQueueCreate(5U, sizeof(movementEvent));
    _timerForMovement = xTimerCreate("Timer to end movement", FORWARD_DELAY_TICKS, pdFALSE, NULL, _endMovementTimerCallback);

    bSERVO_RegisterServo(SERVO_LEFT_GPIO_NUM, PULSE_WIDTH_MIN_MS, PULSE_WIDTH_MAX_MS);
    bSERVO_RegisterServo(SERVO_RIGHT_GPIO_NUM, PULSE_WIDTH_MIN_MS, PULSE_WIDTH_MAX_MS);

    for (;;) {
        xQueueReceive(_queueForMovement, &movementEvent, portMAX_DELAY);

        switch (movementEvent.movement) {
        case MOVEMENT_STOP:
            xTimerStop(_timerForMovement, TIMER_API_DEFAULT_TIMEOUT);
            bSERVO_SetOrder(SERVO_LEFT_GPIO_NUM, SPEED_STOP, true);
            bSERVO_SetOrder(SERVO_RIGHT_GPIO_NUM, SPEED_STOP, true);
            if (_endCallbackToCall != NULL) {
                _endCallbackToCall();
            }
            break;
        case MOVEMENT_FORWARD:
            xTimerChangePeriod(_timerForMovement, FORWARD_DELAY_TICKS, TIMER_API_DEFAULT_TIMEOUT);
            xTimerReset(_timerForMovement, TIMER_API_DEFAULT_TIMEOUT);
            bSERVO_SetOrder(SERVO_LEFT_GPIO_NUM, SPEED_FORWARD, true);
            bSERVO_SetOrder(SERVO_RIGHT_GPIO_NUM, SPEED_FORWARD, true);
            break;
        case MOVEMENT_BACKWARD:
            xTimerChangePeriod(_timerForMovement, BACKWARD_DELAY_TICKS, TIMER_API_DEFAULT_TIMEOUT);
            xTimerReset(_timerForMovement, TIMER_API_DEFAULT_TIMEOUT);
            bSERVO_SetOrder(SERVO_LEFT_GPIO_NUM, SPEED_BACKWARD, false);
            bSERVO_SetOrder(SERVO_RIGHT_GPIO_NUM, SPEED_BACKWARD, false);
            break;
        case MOVEMENT_ROTATION_LEFT:
            xTimerChangePeriod(_timerForMovement, ROTATION_DELAY_TICKS, TIMER_API_DEFAULT_TIMEOUT);
            xTimerReset(_timerForMovement, TIMER_API_DEFAULT_TIMEOUT);
            bSERVO_SetOrder(SERVO_LEFT_GPIO_NUM, SPEED_ROTATION, false);
            bSERVO_SetOrder(SERVO_RIGHT_GPIO_NUM, SPEED_ROTATION, true);
            break;
        case MOVEMENT_ROTATION_RIGHT:
            xTimerChangePeriod(_timerForMovement, ROTATION_DELAY_TICKS, TIMER_API_DEFAULT_TIMEOUT);
            xTimerReset(_timerForMovement, TIMER_API_DEFAULT_TIMEOUT);
            bSERVO_SetOrder(SERVO_LEFT_GPIO_NUM, SPEED_ROTATION, true);
            bSERVO_SetOrder(SERVO_RIGHT_GPIO_NUM, SPEED_ROTATION, false);
            break;
        default:
            break;
        }

        _endCallbackToCall = movementEvent.endCallback;
    }
}

void vMVT_Move(movementType_e movement, void (*endCallback)(void))
{
    movementEvent_t event;

    event.movement = movement;
    event.endCallback = endCallback;

    if (movement == MOVEMENT_STOP) {
        xTimerStop(_timerForMovement, TIMER_API_DEFAULT_TIMEOUT);
        xQueueReset(_queueForMovement);
        xQueueSendToFront(_queueForMovement, &event, WRITE_IN_QUEUE_DEFAULT_TIMEOUT);
    } else {
        xQueueSend(_queueForMovement, &event, WRITE_IN_QUEUE_DEFAULT_TIMEOUT);
    }
}

/* ____________________________________________________________________________ */
/* Static functions 															*/

static void _endMovementTimerCallback(void* timer)
{
    movementEvent_t event;

    event.movement = MOVEMENT_STOP;
    event.endCallback = NULL;

    xQueueSend(_queueForMovement, &event, 0U);
}
