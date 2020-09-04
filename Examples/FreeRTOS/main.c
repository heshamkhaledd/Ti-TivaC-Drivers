#include <stdbool.h>
#include <stdint.h>

#include "FreeRTOS.h"
#include "task.h"
#include "queue.h"
#include "gpio.h"
#include "uart0.h"



static volatile uint8_t flag = 0;

/* Create Task handles for task_1 and task_2 */
TaskHandle_t  task_1_hd, task_2_hd;

/* Initialize Queue handle with NULL */
QueueHandle_t Queue_1 = NULL;

/* Create a struct to hold the data*/
typedef struct
{
    uint8_t c;
}message;

static void task_1(void *pvParameters)
{
    (void)pvParameters; /* Just to remove warnings */

        while (1)
        {

            if (flag == 1)
            {
                /* Receive the Key entered from keyboard */
                uint8_t data = UART0_receiveByte ();

                /* Push the key to the queue */
                xQueueSend (Queue_1,(void *) &data, (TickType_t)0);
            }
        }

        flag = 0;
        vTaskDelay(100);
}



static void task_2(void* pvParameters)
{
    (void)pvParameters; /* Just to remove warnings */

    message received_byte;

    while (1) {

        /* Check if the Queue isn't Empty by using xQueuePeek function */
        if ((xQueuePeek(Queue_1, &received_byte, (TickType_t)10)) != pdFALSE)
        {

            /* If Queue isn't empty, Receive the key from Queue and store it inside the received_byte instance*/
            xQueueReceive(Queue_1, &received_byte, (TickType_t) 10);

            /* Send the key again to PC to show it */
            UART0_sendByte(received_byte.c);
        }

        vTaskDelay(100);
    }
}



void MODULES_init (void)
{
    PIN_CONFIG first_instance  = {PORTF,PIN_1,DIGITAL,OUTPUT,PULL_UP,INTERRUPT_OFF};
    PIN_CONFIG second_instance = {PORTF,PIN_2,DIGITAL,OUTPUT,PULL_UP,INTERRUPT_OFF};
    PIN_CONFIG third_instance  = {PORTF,PIN_3,DIGITAL,OUTPUT,PULL_UP,INTERRUPT_OFF};
    GPIO_configurePIN(&first_instance);
    GPIO_configurePIN(&second_instance);
    GPIO_configurePIN(&third_instance);
    UART0_init ();
}


int main ()
{
        MODULES_init ();

        /* Create a queue and store it's handle in Queue_1 */
        Queue_1 = xQueueCreate(10, sizeof(message*));

        if (Queue_1 == NULL) {
            SET_BIT(PORTF,GPIODATA,PIN_1);
        }
        else {
            SET_BIT(PORTF,GPIODATA,PIN_3);
        }

        xTaskCreate(task_1,"Task_1", 1000, NULL, 1, &task_1_hd);
        xTaskCreate(task_2, "Task_2", 1000, NULL, 0, &task_2_hd);

        vTaskStartScheduler();

}
