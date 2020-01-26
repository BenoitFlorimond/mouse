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

/* ____________________________________________________________________________ */
/* Enum  																		*/
typedef enum {
    SERVO_CONFIG,
    SERVO_CHANGE_COMMAND,
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
} servoCommand_t;

typedef struct {
    servoEvent_e event;
    union {
        servoConfig_t config;
        servoCommand_t command;
    };
} servoEvent_t;

/* ____________________________________________________________________________ */
/* Static prototypes 															*/

/* ____________________________________________________________________________ */
/* Static variables 															*/
static uint8_t _servosNumber = 0;

/* ____________________________________________________________________________ */
/* ISR handlers 																*/

/* ____________________________________________________________________________ */
/* Public functions 															*/
void vSERVO_Process(void* pvParameters)
{
    uint32_t angle, count;
    mcpwm_config_t pwm_config;

    mcpwm_gpio_init(MCPWM_UNIT_0, MCPWM0A, 18);

    //2. initial mcpwm configuration
    printf("Configuring Initial Parameters of mcpwm......\n");
    pwm_config.frequency = 333; //frequency = 50Hz, i.e. for every servo motor time period should be 20ms
    pwm_config.cmpr_a = 0; //duty cycle of PWMxA = 0
    pwm_config.cmpr_b = 0; //duty cycle of PWMxb = 0
    pwm_config.counter_mode = MCPWM_UP_COUNTER;
    pwm_config.duty_mode = MCPWM_DUTY_MODE_0;
    mcpwm_init(MCPWM_UNIT_0, MCPWM_TIMER_0, &pwm_config); //Configure PWM0A & PWM0B with above settings
    mcpwm_set_duty_in_us(MCPWM_UNIT_0, MCPWM_TIMER_0, MCPWM_OPR_A, 2000);
    for (;;) {
        vTaskDelay(pdMS_TO_TICKS(2000));
        mcpwm_set_duty_in_us(MCPWM_UNIT_0, MCPWM_TIMER_0, MCPWM_OPR_A, 1000);
        vTaskDelay(pdMS_TO_TICKS(2000));
        mcpwm_set_duty_in_us(MCPWM_UNIT_0, MCPWM_TIMER_0, MCPWM_OPR_A, 2000);
    }
}
/* ____________________________________________________________________________ */
/* Static functions 															*/
