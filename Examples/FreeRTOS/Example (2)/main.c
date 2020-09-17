#include <stdbool.h>
#include <stdint.h>

#include "FreeRTOS.h"
#include "task.h"
#include "queue.h"
#include "semphr.h"
#include "timers.h"
#include "inc/hw_memmap.h"
#include "inc/hw_types.h"
#include "driverlib/sysctl.h"
#include "driverlib/gpio.h"


#define STD_DELAY   16000000

/* Create Task handles for task_1 and task_2 */
TaskHandle_t  task_1_hd, task_2_hd;

/* Initialize Timer handle with NULL */
TimerHandle_t Timer_1 = NULL;

/* Declare a timer overflow global variable */
uint8_t OVF_ticks = 0;

void SWTimer_Callback (void)
{
    OVF_ticks++;

    /* Change the priorities of task 1 and task 2 */
    if (OVF_ticks == 2)
    {
        uint8_t temp = uxTaskPriorityGet(task_1_hd);

        vTaskPrioritySet(task_1_hd,uxTaskPriorityGet(task_2_hd));
        vTaskPrioritySet(task_2_hd,temp);
        OVF_ticks = 0;

        return;
    }
    else
        return;
}

static void task_1(void *pvParameters)
{
    (void)pvParameters; /* Just to remove warnings */


    while (1)
    {
        GPIOPinWrite (GPIO_PORTF_BASE, GPIO_PIN_2, 0);
        GPIOPinWrite (GPIO_PORTF_BASE, GPIO_PIN_1, 2);

        /* vTaskDelay(100);  No need to suspend the task because the Timer Callback function will change task priorities  */
    }
}

static void task_2(void* pvParameters)
{
    (void)pvParameters; /* Just to remove warnings */


    while (1)
    {
        GPIOPinWrite (GPIO_PORTF_BASE, GPIO_PIN_1, 0);
        GPIOPinWrite (GPIO_PORTF_BASE, GPIO_PIN_2, 4);

        /* vTaskDelay(100);  No need to suspend the task because the Timer Callback function will change task priorities  */
    }
}



void GPIO_init (void)
{
    SysCtlPeripheralEnable(SYSCTL_PERIPH_GPIOF);    /* Enable Clock at PORTF */

    SysCtlDelay (5);    /* A Required delay before starting to operate on any peripheral after Enabling it */

    GPIOPinTypeGPIOOutput (GPIO_PORTF_BASE, GPIO_PIN_1 | GPIO_PIN_2 | GPIO_PIN_3);
}

void MODULES_init (void)
{
    GPIO_init ();
}


int main ()
{
        MODULES_init ();

        /* Create a Timer and store it in the handle */
        Timer_1 = xTimerCreate("Timer_1", pdMS_TO_TICKS(1000), pdTRUE, (void*) 0, (TimerCallbackFunction_t)SWTimer_Callback);

        /* Checking if the SW Timer is created successfully or not
         *
         * If yes, LED will BLINK Green
         * If not, LED will BLINK Red
         */
        if (Timer_1 == NULL) {
            GPIOPinWrite(GPIO_PORTF_BASE,GPIO_PIN_1,2);
        }
        else {
            GPIOPinWrite(GPIO_PORTF_BASE,GPIO_PIN_3,8);
            SysCtlDelay (STD_DELAY);
            GPIOPinWrite (GPIO_PORTF_BASE,GPIO_PIN_3,0);
            SysCtlDelay (STD_DELAY);
        }


        /* Creating the Tasks */
        xTaskCreate(task_1,"Task_1", configMINIMAL_STACK_SIZE, NULL, 2, &task_1_hd);
        xTaskCreate(task_2, "Task_2", configMINIMAL_STACK_SIZE, NULL, 1, &task_2_hd);


        /* Checking if the SW Timer started counting successfully or not
         *
         * If yes, LED will BLINK Green
         * If not, LED will BLINK Red
         */
        if ( xTimerStart(Timer_1, 0) != pdPASS)

            GPIOPinWrite(GPIO_PORTF_BASE,GPIO_PIN_1,2);

        else {
            GPIOPinWrite(GPIO_PORTF_BASE,GPIO_PIN_3,8);
            SysCtlDelay (STD_DELAY);
            GPIOPinWrite (GPIO_PORTF_BASE,GPIO_PIN_3,0);
        }


        vTaskStartScheduler();
}
