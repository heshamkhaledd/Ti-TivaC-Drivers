#include <stdbool.h>
#include <stdint.h>


#include "inc/hw_memmap.h"
#include "inc/hw_types.h"
#include "inc/hw_ints.h"
#include "driverlib/sysctl.h"
#include "driverlib/gpio.h"
#include "driverlib/timer.h"

volatile uint8_t flag = 0;

volatile uint8_t state = 0;

void TIMER_ISR (void)
{
    flag ++;
    TimerIntClear(TIMER0_BASE, TIMER_TIMA_TIMEOUT);
}

void SWITCH_1st_ISR (void)
{
    state = 1;
    GPIOIntClear (GPIO_PORTF_BASE, GPIO_INT_PIN_0);

    SysCtlDelay (5);
}

void SWITCH_2nd_ISR (void)
{
    state = 2;
    GPIOIntClear (GPIO_PORTF_BASE, GPIO_INT_PIN_4);

    SysCtlDelay (5);
}

void TIMER_init (void)
{
    SysCtlPeripheralEnable(SYSCTL_PERIPH_TIMER0);   /* Enable Clock at TIMER0 */
    SysCtlDelay (5);                                /* A Required delay before starting to operate on any peripheral after Enabling it */

    TimerConfigure(TIMER0_BASE, TIMER_CFG_A_PERIODIC_UP | TIMER_CFG_A_ACT_TOINTD);
    TimerPrescaleSet (TIMER0_BASE, TIMER_A, 0xFF);

    TimerIntEnable(TIMER0_BASE, TIMER_TIMA_TIMEOUT);
    TimerIntRegister(TIMER0_BASE, TIMER_A, TIMER_ISR);

    TimerEnable(TIMER0_BASE,TIMER_A);

    return;
}

void TIMER_deinit (void)
{
    TimerDisable (TIMER0_BASE,TIMER_A);

    return;
}

void GPIO_init (void)
{
    SysCtlPeripheralEnable(SYSCTL_PERIPH_GPIOF);    /* Enable Clock at PORTF */
    SysCtlDelay (5);    /* A Required delay before starting to operate on any peripheral after Enabling it */

    GPIOPinTypeGPIOOutput (GPIO_PORTF_BASE, GPIO_PIN_1 | GPIO_PIN_2 | GPIO_PIN_3); /* Enable the Launchpad 3 LEDS */

    GPIOUnlockPin         (GPIO_PORTF_BASE,GPIO_PIN_0 | GPIO_PIN_4);

    GPIOPinTypeGPIOInput  (GPIO_PORTF_BASE, GPIO_PIN_0 | GPIO_PIN_4);   /* Enables the Launchpad 2 Switches */

    GPIOIntTypeSet        (GPIO_PORTF_BASE, GPIO_PIN_0 | GPIO_PIN_4, GPIO_HIGH_LEVEL);
    GPIOIntEnable         (GPIO_PORTF_BASE,GPIO_INT_PIN_0 | GPIO_INT_PIN_4);

    GPIOIntRegisterPin    (GPIO_PORTF_BASE,GPIO_PIN_0 ,SWITCH_1st_ISR);    /* Set the callback function for first switch */
    GPIOIntRegisterPin    (GPIO_PORTF_BASE,GPIO_PIN_4 ,SWITCH_2nd_ISR);    /* Set the callback function for second switch */


}


void MODULES_init (void)
{
    GPIO_init ();
    TIMER_init ();
}



int main ()
{
    MODULES_init ();
    static uint8_t entry = 0;

    while (1)
    {

        if (flag == 1)
        {
            flag = 0;
            if (entry == 0)
            {
                GPIOPinWrite (GPIO_PORTF_BASE, GPIO_PIN_1, 2);
                entry ++;
            }

            else
            {
                GPIOPinWrite (GPIO_PORTF_BASE, GPIO_PIN_1, 0);
                entry = 0;
            }

        }
        else if (flag == 0)
        {
            GPIOPinWrite (GPIO_PORTF_BASE, GPIO_PIN_1, 0);
        }


        if (state == 1)
        {
            TIMER_init ();
            state = 0;
        }
        else if (state == 2)
        {
            TIMER_deinit ();
            state = 0;
        }

    }

}
