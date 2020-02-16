/**
*******************************************************************************
* @file 	servo.c
* @author 	Benoit Florimond
* @date 	10/09/19
*******************************************************************************
*/

/* ____________________________________________________________________________ */
/* Includes  																	*/
#include "servo.h"
#include "driver/mcpwm.h"

/* ____________________________________________________________________________ */
/* Defines  																	*/
#define TAG_SERVO ("SERVO_DRV")
#define MAX_SERVO_NUMBER (12)

/* ____________________________________________________________________________ */
/* Enum  																		*/
typedef enum {
    SERVO_CONFIG,
    SERVO_ORDER,
} servoEvent_e;

/* ____________________________________________________________________________ */
/* Struct																		*/
typedef struct {
    uint32_t gpio;
    uint32_t frequencyHz;
} servoConfig_t;

typedef struct {
    uint32_t gpio;
    uint32_t speedPercentage;
    bool forwardOrder;
} servoCommand_t;

typedef struct {
    servoEvent_e type;
    queueContext_t responseQueue;
    union {
        servoConfig_t config;
        servoCommand_t command;
    };
} servoEvent_t;

typedef struct {
    uint32_t gpio;
    mcpwm_unit_t unit;
    mcpwm_io_signals_t signal;
    mcpwm_operator_t operator;
} servoMapping_t;

/* ____________________________________________________________________________ */
/* Static prototypes 															*/
static bool getIndexFromGpio(uint32_t gpio, uint8_t* servoIndex);

/* ____________________________________________________________________________ */
/* Static variables 															*/
static uint8_t _servosNumber = 0;
static QueueHandle_t _queueForServo = NULL;
static servoMapping_t _servosList[MAX_SERVO_NUMBER] = {
    { 0xFF, MCPWM_UNIT_0, MCPWM0A, MCPWM_OPR_A },
    { 0xFF, MCPWM_UNIT_0, MCPWM0B, MCPWM_OPR_B },
    { 0xFF, MCPWM_UNIT_0, MCPWM1A, MCPWM_OPR_A },
    { 0xFF, MCPWM_UNIT_0, MCPWM1B, MCPWM_OPR_B },
    { 0xFF, MCPWM_UNIT_0, MCPWM2A, MCPWM_OPR_A },
    { 0xFF, MCPWM_UNIT_0, MCPWM2B, MCPWM_OPR_B },
    { 0xFF, MCPWM_UNIT_1, MCPWM0A, MCPWM_OPR_A },
    { 0xFF, MCPWM_UNIT_1, MCPWM0B, MCPWM_OPR_B },
    { 0xFF, MCPWM_UNIT_1, MCPWM1A, MCPWM_OPR_A },
    { 0xFF, MCPWM_UNIT_1, MCPWM1B, MCPWM_OPR_B },
    { 0xFF, MCPWM_UNIT_1, MCPWM2A, MCPWM_OPR_A },
    { 0xFF, MCPWM_UNIT_1, MCPWM2B, MCPWM_OPR_B },
};

/* ____________________________________________________________________________ */
/* ISR handlers 																*/

/* ____________________________________________________________________________ */
/* Public functions 															*/
void vSERVO_Process(void* pvParameters)
{
    servoEvent_t servoEvent = { 0 };
    mcpwm_config_t pwm_config;
    uint8_t index = 0;
    float dutyCycle = 0.0;
    bool result = false;

    _queueForServo = xQueueCreate(10, sizeof(servoEvent_t));

    if (xQueueReceive(_queueForServo, &servoEvent, portMAX_DELAY) == pdTRUE) {
        switch (servoEvent.type) {
        case SERVO_CONFIG:
            if (_servosNumber < MAX_SERVO_NUMBER) {
                _servosList[_servosNumber].gpio = servoEvent.config.gpio;
                mcpwm_gpio_init(_servosList[_servosNumber].unit,
                    _servosList[_servosNumber].signal,
                    servoEvent.config.gpio);
                pwm_config.frequency = servoEvent.config.frequencyHz;
                pwm_config.cmpr_a = 0; //duty cycle of PWMxA = 0
                pwm_config.cmpr_b = 0; //duty cycle of PWMxB = 0
                pwm_config.counter_mode = MCPWM_UP_COUNTER;
                pwm_config.duty_mode = MCPWM_DUTY_MODE_0;
                mcpwm_init(_servosList[_servosNumber].unit, MCPWM_TIMER_0, &pwm_config); //Configure PWM0A & PWM0B with above settings
                _servosNumber++;
                result = true;
            } else {
                result = false;
            }
            vOS_QueueSendSafe(&servoEvent.responseQueue, &result);
            break;
        case SERVO_ORDER:
            if (getIndexFromGpio(servoEvent.command.gpio, &index)) {
                result = true;
                dutyCycle = 50.0 + (servoEvent.command.speedPercentage / 2.0);
                if (!servoEvent.command.forwardOrder) {
                    dutyCycle *= -1.0;
                }
                mcpwm_set_duty(_servosList[index].unit, MCPWM_TIMER_0, _servosList[index].operator, dutyCycle);
            } else {
                result = false;
            }
            vOS_QueueSendSafe(&servoEvent.responseQueue, &result);
            break;
        default:
            break;
        }
    }
}

bool bSERVO_RegisterServo(uint32_t gpio, float minPulseMs, float maxPulseMs)
{
    bool result = false;
    servoEvent_t event = { 0 };

    event.type = SERVO_CONFIG;
    event.config.gpio = gpio;
    event.config.frequencyHz = 1000.0 / (maxPulseMs - minPulseMs);
    if (!bOS_SendToTaskAndWaitResponse(_queueForServo, &event, &event.responseQueue, &result, sizeof(result), TASK_DEFAULT_REPONSE_TIME_TICKS)) {
        ESP_LOGE(TAG_SERVO, "Cannot get response from task");
    }
    return result;
}

bool bSERVO_SetOrder(uint32_t gpio, float speed, bool forward)
{
    bool result = false;
    servoEvent_t event = { 0 };

    event.type = SERVO_ORDER;
    event.command.forwardOrder = forward;
    event.command.gpio = gpio;
    event.command.speedPercentage = speed;
    if (!bOS_SendToTaskAndWaitResponse(_queueForServo, &event, &event.responseQueue, &result, sizeof(result), TASK_DEFAULT_REPONSE_TIME_TICKS)) {
        ESP_LOGE(TAG_SERVO, "Cannot get response from task");
    }
    return result;
}

/* ____________________________________________________________________________ */
/* Static functions 															*/
static bool
getIndexFromGpio(uint32_t gpio, uint8_t* servoIndex)
{
    bool result = false;

    for (uint8_t index = 0; index < MAX_SERVO_NUMBER; index++) {
        if (_servosList[index].gpio == gpio) {
            *servoIndex = index;
            result = true;
            break;
        }
    }

    return result;
}