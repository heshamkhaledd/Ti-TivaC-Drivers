#include <stdbool.h>
#include <stdint.h>
#include "driverlib/gpio.h"
#include "inc/hw_memmap.h"
#include "inc/hw_types.h"
#include "driverlib/sysctl.h"

uint8_t flag = 0;

void change_state (void)
{
    if (flag == 1)
        flag = 0;
    else
        flag++;
}



void config_pins (void)
{
                                            /* CONFIGURE THE SWITCH AT (PORTF,PIN_0) */

    SysCtlPeripheralEnable(SYSCTL_PERIPH_GPIOF);    /* Enable Clock at PORTF */

    SysCtlDelay (5);    /* A Required delay before starting to operate on any peripheral after Enabling it */

    GPIOUnlockPin(GPIO_PORTF_BASE,GPIO_PIN_0);

    GPIOPinTypeGPIOInput(GPIO_PORTF_BASE,GPIO_PIN_0);       /* Unlock the port and sit PIN_0 as input */

    GPIOPadConfigSet(GPIO_PORTF_BASE,GPIO_PIN_0,GPIO_STRENGTH_2MA,GPIO_PIN_TYPE_STD_WPU); /* Specifies the switch strength to 2 mA with Pull-Down Resistor */

    GPIOIntTypeSet(GPIO_PORTF_BASE,GPIO_PIN_0,GPIO_HIGH_LEVEL);   /* Set the interrupt trigger at the falling edge */

    GPIOIntEnable(GPIO_PORTF_BASE,GPIO_INT_PIN_0);                  /* Enable the interrupt at switch */

    GPIOIntRegisterPin(GPIO_PORTF_BASE,GPIO_PIN_0,change_state);    /* Set the callback function for the interrupt */

    GPIOIntRegister(GPIO_PORTF_BASE,change_state);

    GPIOPinTypeGPIOOutput (GPIO_PORTF_BASE,GPIO_PIN_1 | GPIO_PIN_2);

    return;
}


int main(void)

{
    config_pins ();

   while (1)
   {
       if (flag == 0)
       {
           GPIOPinWrite(GPIO_PORTF_BASE,GPIO_PIN_2,0);
           GPIOPinWrite(GPIO_PORTF_BASE,GPIO_PIN_1,2);
       }
       else
       {
           GPIOPinWrite(GPIO_PORTF_BASE,GPIO_PIN_1,0);
           GPIOPinWrite(GPIO_PORTF_BASE,GPIO_PIN_2,4);
       }
   }
}
