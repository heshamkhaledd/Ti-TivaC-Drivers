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
    SysCtlPeripheralEnable(SYSCTL_PERIPH_UART0);
    SysCtlDelay (5);

    SysCtlPeripheralEnable(SYSCTL_PERIPH_GPIOA);
    SysCtlDelay (5);

    GPIOPinTypeUART (GPIO_PORTA_BASE,GPIO_PIN_0 | GPIO_PIN_1);

    UARTConfigSetExpClk(UART0_BASE, SysCtlClockGet (), BAUD_RATE, UART_CONFIG_WLEN_8 | UART_CONFIG_STOP_ONE | UART_CONFIG_PAR_NONE);

    UARTClockSourceSet(UART0_BASE,UART_CLOCK_SYSTEM);

    UARTFIFOEnable (UART0_BASE);

    //UARTFIFOLevelSet(UART0_BASE,UART_FIFO_TX1_8,UART_FIFO_RX1_8);

    UARTIntEnable (UART0_BASE, UART_INT_RT);

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
