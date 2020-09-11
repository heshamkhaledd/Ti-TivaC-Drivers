#include <stdbool.h>
#include <stdint.h>
#include "inc/hw_memmap.h"
#include "inc/hw_types.h"
#include "driverlib/sysctl.h"
#include "driverlib/gpio.h"
#include "driverlib/uart.h"


#define BAUD_RATE   9600

int32_t key = 0;

void UART_ISR (void)
{
        key = UARTCharGet (UART0_BASE);
        UARTCharPut(UART0_BASE,key);
        UARTCharPut(UART0_BASE,'\n');
        UARTCharPut (UART0_BASE,'\r');
        UARTIntClear (UART0_BASE, UART_INT_RT);
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

    return;
}

void UART_sendString (uint8_t *str)
{
    while (*str)
    {
        UARTCharPut(UART0_BASE,(*(str++)));
    }

    return;
}


int main ()
    {
    GPIO_init ();
    UART_init ();
    UART_sendString ("Enter R/G/B \n\r");

    while (1)
    {

        switch (key)
        {
            case 'r':
                GPIOPinWrite(GPIO_PORTF_BASE,GPIO_PIN_2,0);
                GPIOPinWrite(GPIO_PORTF_BASE,GPIO_PIN_3,0);
                GPIOPinWrite(GPIO_PORTF_BASE,GPIO_PIN_1,2);
                break;

            case 'g':
                GPIOPinWrite(GPIO_PORTF_BASE,GPIO_PIN_2,0);
                GPIOPinWrite(GPIO_PORTF_BASE,GPIO_PIN_3,8);
                GPIOPinWrite(GPIO_PORTF_BASE,GPIO_PIN_1,0);
                break;

            case 'b':
                GPIOPinWrite(GPIO_PORTF_BASE,GPIO_PIN_2,4);
                GPIOPinWrite(GPIO_PORTF_BASE,GPIO_PIN_3,0);
                GPIOPinWrite(GPIO_PORTF_BASE,GPIO_PIN_1,0);
                break;

            default:
                GPIOPinWrite(GPIO_PORTF_BASE,GPIO_PIN_2,0);
                GPIOPinWrite(GPIO_PORTF_BASE,GPIO_PIN_3,0);
                GPIOPinWrite(GPIO_PORTF_BASE,GPIO_PIN_1,0);
                break;
        }

    }
}
