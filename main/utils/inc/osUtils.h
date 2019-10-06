/**
******************************************************************************
* @file 	buttons.h
* @author 	Benoit Florimond
* @date 	09/23/19
******************************************************************************
*/

#ifndef __OSUTILS_H__
#define __OSUTILS_H__

#include "system_def.h"

#define vOS_SendToTaskAndWaitResponse(queue, event, response, timeout)      \
    do {                                                                    \
        QueueHandle_t __responseQueue = xQueueCreate(1U, sizeof(response)); \
        event.responseQueue.pQueueHandle = &__responseQueue;                \
        event.responseQueue.expirationTime = timeout;                       \
        event.responseQueue.creationTime = xTaskGetTickCount();             \
        if (queue != NULL) {                                                \
            if (xQueueSend(queue, &event, 0U) == pdTRUE) {                  \
                xQueueReceive(__responseQueue, &response, timeout);         \
            }                                                               \
        }                                                                   \
        vOS_DeleteQueue(__responseQueue);                                   \
    } while (false)

typedef struct {
    QueueHandle_t* pQueueHandle;
    TickType_t creationTime;
    TickType_t expirationTime;
} queueContext_t;

void vOS_DeleteQueue(QueueHandle_t* pQueueToDelete);
void vOS_DeleteSemaphore(SemaphoreHandle_t* pSemaphoreToDelete);
bool bOS_IsQueueReadyForSending(QueueHandle_t queueToSend, uint32_t startTime, uint32_t timeoutValue);
void vOS_CreateResponseQueue(queueContext_t* pQueueContext, QueueHandle_t* pQueue, TickType_t queueTimeout);
void vOS_QueueSendSafe(queueContext_t* pQueueContext, void* item);
bool bOS_SendToTaskAndWaitResponse(QueueHandle_t queueToSend, void* pEvent, queueContext_t* pReponseQueue, void* pResponse, uint8_t reponseSize, portTickType timeout);

#endif //__OSUTILS_H__