/* ========================================
 *
 * Copyright YOUR COMPANY, THE YEAR
 * All Rights Reserved
 * UNPUBLISHED, LICENSED SOFTWARE.
 *
 * CONFIDENTIAL AND PROPRIETARY INFORMATION
 * WHICH IS THE PROPERTY OF your company.
 *
 * ========================================
*/

#include "project.h"
#include "FreeRTOS.h"
#include "semphr.h"
#include "queue.h"
#include <stdio.h>
#include "global.h"

int    Q_Var = 1;

static SemaphoreHandle_t    uartSemaphore;

/* It was already declared in the global.h file */
//QueueHandle_t   xQueue1;

//struct AMessage *pxRxedMessage; 
uint8_t pxRxedMessage;

static void UART_Isr()
{       
    Cy_SCB_SetRxInterruptMask(UART_1_HW, 0); /* This register configures which
    bits from the RX interrupt request register can trigger an interrupt event. */
    
    Cy_SCB_ClearRxInterrupt(UART_1_HW, CY_SCB_RX_INTR_NOT_EMPTY); /* The RX interrupt sources
    to be cleared. See RX Interrupt Statuses for the set of constants */
    
    NVIC_ClearPendingIRQ((IRQn_Type) UART_1_SCB_IRQ_cfg.intrSrc);
    /* Function to access the Nested Vector Interrupt Controller (NVIC). This function removes
    the pending state of the specified device specific interrupt IRQn.
    IRQn cannot be a negative number.) */
    
    BaseType_t xHigherPriorityTaskWoken;
    xHigherPriorityTaskWoken = pdFALSE;
    
    /* Unblock the task by releasing the semaphore. */
    xSemaphoreGiveFromISR( uartSemaphore, /* A handle to the semaphore being released. This is the handle returned when the semaphore was created. */
                           &xHigherPriorityTaskWoken);
    /* Macro to release a semaphore. */
    
    /* If xHigherPriorityTaskWoken was set to true you we should yield.  
    The actual macro used here is port specific. */
    if(xHigherPriorityTaskWoken != pdFALSE)
        portYIELD_FROM_ISR( xHigherPriorityTaskWoken );          
}

void uartTask(void *arg)
{
    (void)arg;
    //UART_1_Start();
    //setvbuf( stdin, NULL, _IONBF, 0);
    printf("Started Task\r\n");
    
    uartSemaphore = xSemaphoreCreateBinary();
    
    (void) Cy_SysInt_Init(&UART_1_SCB_IRQ_cfg, /* Interrupt configuration structure */ 
                          &UART_Isr);          /* Address of the ISR */
    
    NVIC_EnableIRQ((IRQn_Type) UART_1_SCB_IRQ_cfg.intrSrc);
    Cy_SCB_SetRxInterruptMask(UART_1_HW,CY_SCB_RX_INTR_NOT_EMPTY);
    
    /* Need to send the initial GREEN LED period, to avoid errors */
    if( xQueueSend( xQueue1, 
                ( void * ) &Q_Var,
                ( TickType_t ) 1000 ) != pdPASS )
    {
        printf("Failed to post the message, even after 100mS\r\n");
    } 
    /*********************************************/
    
    while (1)
    {
        xSemaphoreTake(uartSemaphore,portMAX_DELAY);
        
        while(Cy_SCB_UART_GetNumInRxFifo(UART_1_HW))
        {
            char c = getchar();
            switch(c)
            {
                case 'a':
                    printf("'a' was pressed\r\n");
                    
                    /* Increase Q_Var integer to be send */
                    Q_Var++;
                    
                    xTaskNotifyGive( xHandle1 );
                    
                if( xQueue1 != 0 )
                    {
                        /* Wait for 10 ticks for space to become available if necessary. */
                        if( xQueueSend( xQueue1, 
                                        (void *) &Q_Var,
                                        ( TickType_t ) 10 ) != pdPASS )
                        {
                            printf("Failed to post the message, even after 100mS\r\n");
                        } 
                        else
                        {  
                            /* Send a notification to prvTask2(), bringing it out of the Blocked
                            state. */
                            printf("Increased value: %d was Q_Send to the GREEN LED function\r\n", Q_Var);
                        }  
                    }
                    ulTaskNotifyTake( pdTRUE, portMAX_DELAY );
                break;
                    
                case 'c':
                    printf("'c' was pressed\r\n");
                    Q_Var = 1;
                break;                                  
            }
            
        }
        
        Cy_SCB_SetRxInterruptMask(UART_1_HW,CY_SCB_RX_INTR_NOT_EMPTY);
        
        if( xQueuemsg != 0 )
            {
                // Receive a message on the created queue.  Block for 10 ticks if a
                // message is not immediately available.
                if( xQueueReceive( xQueuemsg, &( pxRxedMessage ), ( TickType_t ) 10 ) )
                {
                    printf("Message received\r\n");
                    printf("*ptr address 1: %d\r\n", pxRxedMessage);
                    printf("*ptr address 2: %d\r\n", pxRxedMessage+1);
                }
            }        
    } 
}

/* [] END OF FILE */
