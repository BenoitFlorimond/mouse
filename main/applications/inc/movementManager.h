/**
*******************************************************************************
* @file 	movementManagers.h
* @author 	Benoit Florimond
* @date 	02/17/20
*******************************************************************************
*/

#ifndef __MOVEMENTMANAGER_H__
#define __MOVEMENTMANAGER_H__

/* ____________________________________________________________________________ */
/* Includes 																	*/
#include "system_def.h"

/* ____________________________________________________________________________ */
/* Defines 																		*/

/* ____________________________________________________________________________ */
/* Enum 																		*/
typedef enum {
    MOVEMENT_STOP = 0,
    MOVEMENT_FORWARD,
    MOVEMENT_BACKWARD,
    MOVEMENT_ROTATION_LEFT,
    MOVEMENT_ROTATION_RIGHT,
} movementType_e;

/* ____________________________________________________________________________ */
/* Struct																	 	*/

/* ____________________________________________________________________________ */
/* Public function prototypes 													*/
void vMVT_Process(void* pvParameters);

void vMVT_Move(movementType_e movement, void (*endCallback)(void));

#endif //__MOVEMENTMANAGER_H__