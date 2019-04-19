/******************************************************************************
* File Name: main_cm4.c
*
* Version 2.0
*
* Description:
*  This code example demonstrates the use of GPIO configured as an input pin to
*  generate interrupts on CM4 CPU in PSoC 6 MCU.
*
* Related Document: CE219521.pdf
*******************************************************************************/
#include "project.h"
#include "FreeRTOS.h"
#include "task.h"
#include "queue.h"
#include <stdio.h>
#include <limits.h>
#include "semphr.h"
#include "timers.h"
#include "uartTask.h"
#include "cpuSleepLocal.h"
#include "GreenLedBlink.h"
#include "global.h"

/*******************************************************************************
* Function Name: main
********************************************************************************
*
* Summary:
*  System entrance point. This function configures and initializes the GPIO
*  interrupt, update the delay on every GPIO interrupt, blinks the LED and enter
*  in deepsleep mode.
*
* Parameters: None
*
* Return: int
*
*******************************************************************************/

/* Prototypes of the two tasks created by main(). */
TaskHandle_t  xHandle;
TaskHandle_t  xHandle1;

QueueHandle_t xQueue1; 

int main(void)
{
    /* Enale interrupt */
    __enable_irq();
    
    /***** Message queue struct *****/
    //extern struct  AMessage *pxMessage;
    /********************************/
    
    UART_1_Start();
    setvbuf( stdin, NULL, _IONBF, 0);
    printf("GitHUB Lesson for Saturday.\r\n");
    
    /********* Create and initialize the main queue here ********/
    xQueue1 = xQueueCreate( 1,            /* Number of elements */
                            sizeof(int)); /* element size       */
    
    if( xQueue1 == NULL ) {
        /* Queue was not created and must not be used. */
        printf("Queue was not created\r\n");
    }  
    /************************************************************/
    
    /* Task Declarations */
    xTaskCreate( uartTask,          /* Function that implements the task. */
                "UART Task",        /* Text name for the task. */   
                400,                /* Stack size in words, not bytes. */
                0,                  /* Parameter passed into the task. */
                tskIDLE_PRIORITY,   /* Priority at which the task is created. */
                &xHandle );         /* Used to pass out the created task's handle. */
    
    //xTaskCreate( cpuSleepLocal, "cpu Sleep Local",  400, 0, 2, 0);
    
    xTaskCreate(
                GreenLedBlink,       /* Function that implements the task. */
                "GreenLedBlink",     /* Text name for the task. */
                400,                 /* Stack size in words, not bytes. */
                0,                   /* Parameter passed into the task. */
                tskIDLE_PRIORITY,    /* Priority at which the task is created. */
                &xHandle1 );         /* Used to pass out the created task's handle. */
          
    vTaskStartScheduler();
        
	for(;;)
	{
        //
	}
}

/* [] END OF FILE */
