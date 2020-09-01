#include "gpio.h"
#include "uart0.h"

PIN_CONFIG first_instance  = {PORTF,PIN_1,DIGITAL,OUTPUT,PULL_UP,INTERRUPT_OFF};
PIN_CONFIG second_instance = {PORTF,PIN_2,DIGITAL,OUTPUT,PULL_UP,INTERRUPT_OFF};
PIN_CONFIG third_instance  = {PORTF,PIN_3,DIGITAL,OUTPUT,PULL_UP,INTERRUPT_OFF};



int main(void)
{

    GPIO_configurePIN(&first_instance);
    GPIO_configurePIN(&second_instance);
    GPIO_configurePIN(&third_instance);
    ACCESS_REG(PORTF,GPIODATA) &= ~((1<<PIN_0) | (1<<PIN_1) | (1<<PIN_2));
    UART0_init();
    uint8_t byte_recv;
    uint8_t flag = 0;

   while (1)
   {
        UART0_sendString ("R/G/B \n\r");
        byte_recv = UART0_receiveByte();
        UART0_sendString ("\n\r");

        switch (byte_recv)
        {
            case 'r':
                if (flag != 0)
                {
                    switch (flag)
                    {
                        case 1:
                            break;
                        case 2:
                            CLEAR_BIT (PORTF,GPIODATA,PIN_2);
                            break;
                        case 3:
                            CLEAR_BIT (PORTF,GPIODATA,PIN_3);
                            break;
                    }
                }

                SET_BIT(PORTF,GPIODATA,PIN_1);
                flag = 1;
                break;
            case 'g':
                if (flag != 0)
                {
                    switch (flag)
                    {
                        case 1:
                            CLEAR_BIT (PORTF,GPIODATA,PIN_1);
                            break;
                        case 2:
                            CLEAR_BIT (PORTF,GPIODATA,PIN_2);
                            break;
                        case 3:
                            break;
                    }
                }

                SET_BIT(PORTF,GPIODATA,PIN_3);
                flag = 3;
                break;
            case 'b':
                if (flag != 0)
                {
                    switch (flag)
                     {
                         case 1:
                             CLEAR_BIT (PORTF,GPIODATA,PIN_1);
                             break;
                         case 2:
                             break;
                         case 3:
                             CLEAR_BIT (PORTF,GPIODATA,PIN_2);
                             break;
                     }
                }

                SET_BIT(PORTF,GPIODATA,PIN_2);
                flag = 2;
                break;
            default:
                ACCESS_REG(PORTF,GPIODATA) &= ~((1<<PIN_0) | (1<<PIN_1) | (1<<PIN_2));
                break;
        }

    }
}

