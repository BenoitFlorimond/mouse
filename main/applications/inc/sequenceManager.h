/**
*******************************************************************************
* @file 	sequenceManager.h
* @author 	Benoit Florimond
* @date 	02/19/20
*******************************************************************************
*/

#ifndef __SEQUENCEMANAGER_H__
#define __SEQUENCEMANAGER_H__

/* ____________________________________________________________________________ */
/* Includes 																	*/
#include "movementManager.h"
#include "system_def.h"

/* ____________________________________________________________________________ */
/* Defines 																		*/

/* ____________________________________________________________________________ */
/* Enum 																		*/

/* ____________________________________________________________________________ */
/* Struct																	 	*/

/* ____________________________________________________________________________ */
/* Public function prototypes 													*/
void vSEQMNGR_Process(void* pvParameters);

void vSEQMNGR_AddNewMovement(movementType_e movement);

void vSEQMNGR_RemoveLastMovement(void);

void vSEQMNGR_LaunchSequence(void);

void vSEQMNGR_AbortSequence(void);

#endif //__SEQUENCEMANAGER_H__