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
#include "queue.h"
#include <stdio.h>
#include "global.h"

/* Macros for configuring project functionality */
#define LED_ON                  (0)
#define LED_OFF                 (1)
#define LED_DELAY               25

int led_delay_local = LED_DELAY;
int pxRxedMessage; /* Don't edit this int for now */

/* structure pointer to be send using that  */
struct AMessage
 {
    uint8_t msg1;
    uint8_t msg2;
 } xMessage;

QueueHandle_t xQueuemsg;

void GreenLedBlink(void *arg)
{
    (void)arg;
    printf("Started GreenLedBlink Task\r\n");
    
    xMessage.msg1 = 89;
    xMessage.msg2 = 12;
    
    struct AMessage *pxMessage;
    
    // Create a queue capable of containing 1 pointer to AMessage structures.
    // These should be passed by pointer as they contain a lot of data.
    xQueuemsg = xQueueCreate( 1, sizeof( struct AMessage * ) );
    if( xQueuemsg == 0 )
    {
        printf("Failed to create the queue\r\n");
    } 
    pxMessage = & xMessage;
 
	for(;;)
	{     
        if( xQueue1 != 0 )
        {
        /* Block to wait for uartTask() to notify this task. */
        ulTaskNotifyTake( pdTRUE, portMAX_DELAY ); 
            
            // Receive a message on the created queue.  Block for 100 ticks if a
            // message is not immediately available.
            if( xQueueReceive( xQueue1, &( pxRxedMessage ), ( TickType_t ) 100 ) )
            {
                printf("New Q Value received: %d\r\n", pxRxedMessage);   
                led_delay_local = pxRxedMessage*LED_DELAY;
            }
            else    
            {
                printf("Error time out receiving Q value\r\n");   
            }  
        
        /* Send a message with pointer's structure */
        printf("GreenLedBlink sent a message with pointer's structure\r\n");
        printf("msg1: %d msg2: %d\r\n", xMessage.msg1, xMessage.msg2);
       
        xQueueSend( xQueuemsg, ( void * ) &pxMessage, ( TickType_t ) 0 );
            
        /* Time to set the LED function to bck to idle priority*/
        vTaskPrioritySet( NULL, tskIDLE_PRIORITY );
        /********************************************************/
        }
        
        /* Send a notification to uartTask(), bringing it out of the Blocked state. */
        xTaskNotifyGive( xHandle );  
        /****************************************************************************/
        
        printf("GREED LED new delay: %d\r\n", led_delay_local); 
        
        for(int dl=0; dl <= 5; dl++)
        {           
    		CyDelay(led_delay_local);
            Cy_GPIO_Write(GREEN_LED_PORT, GREEN_LED_NUM, LED_OFF);
    		CyDelay(led_delay_local);
            Cy_GPIO_Write(GREEN_LED_PORT, GREEN_LED_NUM, LED_ON);
        }
    }
}

/* [] END OF FILE */
