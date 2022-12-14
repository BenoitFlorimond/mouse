/**
*******************************************************************************
* @file 	sequenceManager.c
* @author 	Benoit Florimond
* @date 	02/19/20
*******************************************************************************
*/

/* ____________________________________________________________________________ */
/* Includes  																	*/
#include "sequenceManager.h"

/* ____________________________________________________________________________ */
/* Defines  																	*/
#define SEQ_MNGR_TAG ("SEQ_MNGR")
#define SEQUENCE_MAX_SIZE (50U)

/* ____________________________________________________________________________ */
/* Enum  																		*/
typedef enum {
    ADD_NEW_MOVEMENT,
    REMOVE_LAST_MOVEMENT,
    LAUNCH_SEQUENCE,
    ABORT_SEQUENCE,
    END_OF_CURRENT_MOVEMENT,
} sequenceEvent_e;

/* ____________________________________________________________________________ */
/* Struct																		*/
typedef struct {
    sequenceEvent_e type;
    movementType_e movement;
} sequenceEvent_t;

/* ____________________________________________________________________________ */
/* Static prototypes 															*/
void endOfMovementCallback(void);

/* ____________________________________________________________________________ */
/* Static variables 															*/
static QueueHandle_t _queueForSequence = NULL;
static movementType_e _sequence[SEQUENCE_MAX_SIZE] = { [0 ... SEQUENCE_MAX_SIZE - 1] = MOVEMENT_STOP };
static uint8_t _sequenceLength = 0U;
static uint8_t _sequenceReadIndex = 0U;

/* ____________________________________________________________________________ */
/* ISR handlers 																*/

/* ____________________________________________________________________________ */
/* Public functions 															*/
void vSEQMNGR_Process(void* pvParameters)
{
    sequenceEvent_t event = { 0 };

    _queueForSequence = xQueueCreate(5U, sizeof(event));

    for (;;) {
        xQueueReceive(_queueForSequence, &event, portMAX_DELAY);

        switch (event.type) {
        case ADD_NEW_MOVEMENT:
            _sequence[_sequenceLength++] = event.movement;
            ESP_LOGI(SEQ_MNGR_TAG, "New movement added: %d (%d steps)", event.movement, _sequenceLength);
            break;
        case REMOVE_LAST_MOVEMENT:
            if (_sequenceLength > 0U) {
                _sequenceLength--;
                ESP_LOGI(SEQ_MNGR_TAG, "Last movement has been removed (%d steps left)", _sequenceLength);
            } else {
                ESP_LOGE(SEQ_MNGR_TAG, "Impossible to remove last movement, sequence is empty");
            }
            break;
        case ABORT_SEQUENCE:
            vMVT_Move(MOVEMENT_STOP, NULL);
            _sequenceLength = 0U;
            ESP_LOGI(SEQ_MNGR_TAG, "Sequence aborted");
            break;
        case LAUNCH_SEQUENCE:
            _sequenceReadIndex = 0U;
            if (_sequenceLength > 0U) {
                vMVT_Move(_sequence[_sequenceReadIndex++], endOfMovementCallback);
                ESP_LOGI(SEQ_MNGR_TAG, "Sequence launched (%d steps)", _sequenceLength);
            } else {
                ESP_LOGE(SEQ_MNGR_TAG, "Impossible to launch sequence, it is empty");
            }
            break;
        case END_OF_CURRENT_MOVEMENT:
            if (_sequenceReadIndex < _sequenceLength) {
                ESP_LOGI(SEQ_MNGR_TAG, "Next movement: %d (index %d)", _sequence[_sequenceReadIndex], _sequenceReadIndex);
                vMVT_Move(_sequence[_sequenceReadIndex++], endOfMovementCallback);
            } else {
                _sequenceReadIndex = 0U;
                ESP_LOGI(SEQ_MNGR_TAG, "END OF SEQUENCE");
            }
            break;
        default:
            break;
        }
    }
}

void vSEQMNGR_AddNewMovement(movementType_e movement)
{
    sequenceEvent_t event;

    event.type = ADD_NEW_MOVEMENT;
    event.movement = movement;

    xQueueSend(_queueForSequence, &event, WRITE_IN_QUEUE_DEFAULT_TIMEOUT);
}

void vSEQMNGR_RemoveLastMovement(void)
{
    sequenceEvent_t event;

    event.type = REMOVE_LAST_MOVEMENT;

    xQueueSend(_queueForSequence, &event, WRITE_IN_QUEUE_DEFAULT_TIMEOUT);
}

void vSEQMNGR_LaunchSequence(void)
{
    sequenceEvent_t event;

    event.type = LAUNCH_SEQUENCE;

    xQueueSend(_queueForSequence, &event, WRITE_IN_QUEUE_DEFAULT_TIMEOUT);
}

void vSEQMNGR_AbortSequence(void)
{
    sequenceEvent_t event;

    event.type = ABORT_SEQUENCE;

    xQueueSend(_queueForSequence, &event, WRITE_IN_QUEUE_DEFAULT_TIMEOUT);
}

/* ____________________________________________________________________________ */
/* Static functions 															*/

void endOfMovementCallback(void)
{
    sequenceEvent_t event;

    event.type = END_OF_CURRENT_MOVEMENT;

    xQueueSend(_queueForSequence, &event, WRITE_IN_QUEUE_DEFAULT_TIMEOUT);
}
