#ifndef __SYSTEMDEF_H__
#define __SYSTEMDEF_H__

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "esp_log.h"
#include "esp_system.h"

#include "freertos/FreeRTOS.h"

#include "freertos/queue.h"
#include "freertos/semphr.h"
#include "freertos/task.h"
#include "freertos/timers.h"

#include "osUtils.h"

#define WRITE_IN_QUEUE_DEFAULT_TIMEOUT (pdMS_TO_TICKS(10))
#define TASK_DEFAULT_REPONSE_TIME_TICKS (pdMS_TO_TICKS(100U))

#endif //__SYSTEMDEF_H__
