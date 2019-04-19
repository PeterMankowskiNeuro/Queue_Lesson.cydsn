/* ========================================
 *
 * Copyright YOUR COMPANY, THE YEAR
 * All Rights Reserved
 * UNPUBLISHED, LICENSED SOFTWARE.
 *
 * CONFIDENTIAL AND PROPRIETARY INFORMATION
 * WHICH IS THE PROPERTY OF Novela Neuro.
 * Author: Peter Mankowski
 * ========================================
*/
#pragma     once

#include    "FreeRTOS.h"
#include    "queue.h" 
#include    "semphr.h"
#include    "task.h"

extern  QueueHandle_t xQueue1; 

extern TaskHandle_t  xHandle;
extern TaskHandle_t  xHandle1;

extern QueueHandle_t xQueuemsg;

/* [] END OF FILE */
