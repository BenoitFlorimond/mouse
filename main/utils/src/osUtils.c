/**
******************************************************************************
* @file 	osUtils.c
* @author 	Benoit Florimond
* @date 	09/29/19
******************************************************************************
*/

#include "osUtils.h"

void vOS_DeleteQueue(QueueHandle_t* pQueueToDelete)
{
    QueueHandle_t tempHandle;

    tempHandle = *pQueueToDelete;
    *pQueueToDelete = NULL;
    vQueueDelete(tempHandle);
}

void vOS_DeleteSemaphore(SemaphoreHandle_t* pSemaphoreToDelete)
{
    SemaphoreHandle_t tempHandle;

    tempHandle = *pSemaphoreToDelete;
    *pSemaphoreToDelete = NULL;
    vSemaphoreDelete(tempHandle);
}

bool bOS_IsQueueReadyForSending(QueueHandle_t queueToSend, uint32_t startTime, uint32_t timeoutValue)
{
    bool result = false;

    if (timeoutValue == (uint32_t)portMAX_DELAY) {
        /* Should not happen but check it anyway */
        if (queueToSend != NULL) {
            result = true;
        }
    } else {
        if (xTaskGetTickCount() - startTime < timeoutValue) {
            if (queueToSend != NULL) {
                result = true;
            }
        }
    }

    return result;
}

void vOS_CreateResponseQueue(queueContext_t* pQueueContext, QueueHandle_t* pQueue, TickType_t queueTimeout)
{
    pQueueContext->pQueueHandle = pQueue;
    pQueueContext->expirationTime = queueTimeout;
    pQueueContext->creationTime = xTaskGetTickCount();
}

void vOS_QueueSendSafe(queueContext_t* pQueueContext, void* item)
{
    if (bOS_IsQueueReadyForSending(*pQueueContext->pQueueHandle, pQueueContext->creationTime, pQueueContext->expirationTime)) {
        xQueueSend(*pQueueContext->pQueueHandle, item, 0U);
    }
}

bool bOS_SendToTaskAndWaitResponse(QueueHandle_t queueToSend, void* pEvent, queueContext_t* pReponseQueue, void* pResponse, uint8_t reponseSize, portTickType timeout)
{
    bool result = false;
    QueueHandle_t responseQueue = xQueueCreate(1U, reponseSize);

    pReponseQueue->pQueueHandle = &responseQueue;
    pReponseQueue->expirationTime = timeout;
    pReponseQueue->creationTime = xTaskGetTickCount();
    if (queueToSend != NULL) {
        if (xQueueSend(queueToSend, pEvent, 0U) == pdTRUE) {
            if (xQueueReceive(responseQueue, pResponse, timeout) == pdTRUE) {
                result = true;
            }
        }
    }
    vOS_DeleteQueue(&responseQueue);

    return result;
}
