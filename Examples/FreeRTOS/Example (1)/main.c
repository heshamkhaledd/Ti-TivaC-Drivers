#include <stdbool.h>
#include <stdint.h>

#include "FreeRTOS.h"
#include "task.h"
#include "queue.h"
#include "semphr.h"
#include "inc/hw_memmap.h"
#include "inc/hw_types.h"
#include "driverlib/sysctl.h"
#include "driverlib/gpio.h"
#include "driverlib/uart.h"


#define BAUD_RATE   9600

/* Create Task handles for task_1 and task_2 */
TaskHandle_t  task_1_hd, task_2_hd;

/* Initialize Queue handle with NULL */
QueueHandle_t Queue_1 = NULL;

/* Initialize Semaphore handle with NULL */
SemaphoreHandle_t Semaphore_1 = NULL;

/* A global variable to Receive the data from the Queue and Send it to PC using UART */
uint8_t data = 0;

void UART_ISR (void)
{
    /* Give back the semaphore for Task 1 */
    xSemaphoreGiveFromISR (Semaphore_1,NULL);

    /* Clearing UART Interrupt Receive Timeout Flag */
    UARTIntClear (UART0_BASE,UART_INT_RT);
}

static void task_1(void *pvParameters)
{
    (void)pvParameters; /* Just to remove warnings */

    uint8_t var;

    while (1)
    {
        /* Wait the ISR to aquire the semaphore */
        xSemaphoreTake (Semaphore_1,(TickType_t )100);

        /* Receive the Key entered from keyboard */
        var = UARTCharGet(UART0_BASE);

        /* Push the key to the queue */
        xQueueSend(Queue_1, (void* ) &var, (TickType_t )0);
        vTaskDelay(100);
    }
}

static void task_2(void* pvParameters)
{
    (void)pvParameters; /* Just to remove warnings */


    while (1) {

        /* Check if the Queue isn't Empty by using xQueuePeek function */
        if ((xQueuePeek(Queue_1,(void *) &data, (TickType_t)10)) != pdFALSE)
        {

            /* If Queue isn't empty, Receive the key from Queue and store it inside the received_byte instance*/
            xQueueReceive(Queue_1, (void *) &data, (TickType_t) 10);

            /* Send the key again to PC to show it */
            UARTCharPut(UART0_BASE,data);
        }

        vTaskDelay(100);
    }
}


void UART_init (void)
{
    SysCtlPeripheralEnable(SYSCTL_PERIPH_UART0);    /* Generate Clock to UART0 */
    SysCtlDelay (5);        /* Required Delay for the SysCtlPeripheralEnable function
                                Can be replaced with Polling on the SysCtl State */

    SysCtlPeripheralEnable(SYSCTL_PERIPH_GPIOA);    /* Generate Clock to UART0 */

    SysCtlDelay (5);                                /* Required Delay for the SysCtlPeripheralEnable function
                                                       Can be replaced with Polling on the SysCtl State */


    /* Configure UART Pins as output */
    GPIOPinTypeUART (GPIO_PORTA_BASE,GPIO_PIN_0 | GPIO_PIN_1);


    /* Configure The UART0 to work on
     * 1. 16 MHz Clock
     * 2. Baud Rate of 9600
     * 3. Word Length of 8-bit (1 byte)
     * 4. One stop bit
     * 5. No Parity
     * 6. Double speed-mode
     */
    UARTConfigSetExpClk(UART0_BASE, SysCtlClockGet (), BAUD_RATE, UART_CONFIG_WLEN_8 | UART_CONFIG_STOP_ONE | UART_CONFIG_PAR_NONE);

    /* Re-configure the UART Clock source to the System Clock (Because of the Double speed mode) */
    UARTClockSourceSet(UART0_BASE,UART_CLOCK_SYSTEM);

    /* Enable the FIFO Buffer */
    UARTFIFOEnable (UART0_BASE);

    /* Enable the UART Interrupt at the Receive Timeout flag*/
    UARTIntEnable (UART0_BASE, UART_INT_RT);

    /* Assign a callback function to act as the ISR to the UART */
    UARTIntRegister (UART0_BASE, UART_ISR);

    return;
}


void GPIO_init (void)
{
    SysCtlPeripheralEnable(SYSCTL_PERIPH_GPIOF);    /* Enable Clock at PORTF */

    SysCtlDelay (5);    /* A Required delay before starting to operate on any peripheral after Enabling it */

    GPIOPinTypeGPIOOutput (GPIO_PORTF_BASE,GPIO_PIN_1 | GPIO_PIN_2 | GPIO_PIN_3);
}

void MODULES_init (void)
{
    UART_init ();
    GPIO_init ();
}


int main ()
{
        MODULES_init ();

        /* Create a Semaphore and store it's handle in Semaphore_1 */
        Semaphore_1 = xSemaphoreCreateBinary ();

        /* Create a queue and store it's handle in Queue_1 */
        Queue_1 = xQueueCreate(10, sizeof(uint8_t));

        /* Soft check if the Queue is created
         *
         * If created, LED will blink Green
         * If not, LED will blink Red
        */
        if (Queue_1 == NULL) {
            GPIOPinWrite(GPIO_PORTF_BASE,GPIO_PIN_1,2);
        }
        else {
            GPIOPinWrite(GPIO_PORTF_BASE,GPIO_PIN_3,8);
            SysCtlDelay (16000000);
            GPIOPinWrite (GPIO_PORTF_BASE,GPIO_PIN_3,0);
        }


        /* Creating the Tasks */
        xTaskCreate(task_1,"Task_1", 1000, NULL, 2, &task_1_hd);
        xTaskCreate(task_2, "Task_2", 1000, NULL, 1, &task_2_hd);


        vTaskStartScheduler();

}
