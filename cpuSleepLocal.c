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
#include <stdio.h>

/* Macros for configuring project functionality */
#define DELAY_SHORT             (50)   /* milliseconds */
#define DELAY_LONG              (300)   /* milliseconds */

#define LED_BLINK_COUNT		    (6)

#define LED_ON                  (0)
#define LED_OFF                 (1)

#define SWITCH_INTR_PRIORITY	(3u)

/* Global variable */
uint32_t interrupt_flag   = false;
uint8_t  go_flag          = 0; 
int      isr_get_priotity = 0;
uint32_t count            = 0;
uint32_t delayMs          = DELAY_LONG;

/*******************************************************************************
* Function Name: void Isr_switch(void)
********************************************************************************
*
* Summary:
*  This function is executed when GPIO interrupt is triggered.
*
* Parameters: None
*
* Return: None
*
*******************************************************************************/
void Isr_switch(void)
{
    /* Clears the triggered pin interrupt */
	Cy_GPIO_ClearInterrupt(User_SW_0_PORT, User_SW_NUM);
    NVIC_ClearPendingIRQ(isr_local_1_cfg.intrSrc);
    
    /* Get current priority level */
    isr_get_priotity = NVIC_GetPriority(isr_local_1_cfg.intrPriority);
    
    printf("SW pressed with isr priority level: %d \r\n", isr_get_priotity);
    
    /* Set interrupt flag */
	interrupt_flag = true;        
}

void cpuSleepLocal( void *arg)
{
    (void)arg;

    printf("Started cpuSleepLocal Task\r\n");
    
/* Initialize and enable GPIO interrupt assigned to CM4 */
      Cy_SysInt_Init(
                    &isr_local_1_cfg, /* Interrupt configuration structure */
                     Isr_switch);     /* Address of the ISR */
    
    NVIC_ClearPendingIRQ(isr_local_1_cfg.intrSrc);
    NVIC_EnableIRQ(isr_local_1_cfg.intrSrc); 
 
	for(;;)
	{
        /* Check the interrupt status */
		if(true == interrupt_flag)
		{
			interrupt_flag = false;
            go_flag++; 
            
            /* Update the LED blink interval */
			if(DELAY_LONG == delayMs)
			{
				delayMs = DELAY_SHORT;   
			}
			else
			{
				delayMs = DELAY_LONG;
			}
		}

        /* Blink RED LED four times */
		for (count = 0; count < LED_BLINK_COUNT; count++)
		{
			Cy_GPIO_Write(RED_LED_PORT, RED_LED_NUM, LED_ON);
			Cy_SysLib_Delay(delayMs);
			Cy_GPIO_Write(RED_LED_PORT, RED_LED_NUM, LED_OFF);
			Cy_SysLib_Delay(delayMs);
		}
        printf("User switch was pressed %d times\r\n", go_flag);
        printf("Done...going to CPU sleep\r\n");
        
		/* Enter deep sleep mode */
		//Cy_SysPm_DeepSleep(CY_SYSPM_WAIT_FOR_INTERRUPT);
	}
}

/* [] END OF FILE */
