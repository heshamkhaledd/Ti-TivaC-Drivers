/******************************************************************************
 *
 * Module: GPIO
 *
 * File Name: gpio.c
 *
 * Description: Source file for the GPIO driver
 *
 * Engineer: Hesham Khaled
 *
 *******************************************************************************/

#include "gpio.h"


/******************************************************************************
 *
 * Function Name: GPIO_configurePin
 *
 * Description: A function that's responsible for initializing the required Pin
 *              with the given configuration
 *
 * Input: PIN_CONFIG const *CONFIG_Ptr
 * Output: void
 *
 *****************************************************************************/
void GPIO_configurePin (PIN_CONFIG const *CONFIG_Ptr) {

                        /* Initialize Clock for the required Port */
    GPIO_initClock (CONFIG_Ptr -> PORT);

                        /* Set The Direction of the required PIN */
    ACCESS_REG((CONFIG_Ptr -> PORT), GPIODIR) |= ((CONFIG_Ptr -> DIRECTION) << (CONFIG_Ptr -> PIN));

                        /* Set The Behaviour of the required PIN */
    if (CONFIG_Ptr -> BEHAVIOUR == DIGITAL)
        ACCESS_REG((CONFIG_Ptr -> PORT), GPIODEN) |= (1 << (CONFIG_Ptr -> PIN));

    else if (CONFIG_Ptr -> BEHAVIOUR == ANALOG)
        ACCESS_REG((CONFIG_Ptr -> PORT), GPIOAMSEL) |= (1 << (CONFIG_Ptr -> PIN));

                        /* Set the Logic of the required PIN */
    if (CONFIG_Ptr -> LOGIC == PULL_UP)
        ACCESS_REG((CONFIG_Ptr -> PORT), GPIOUR) |= ( 1 << (CONFIG_Ptr -> PIN));

    else if (CONFIG_Ptr -> LOGIC == PULL_DOWN)
        ACCESS_REG((CONFIG_Ptr -> PORT), GPIOUD) |= ( 1 << (CONFIG_Ptr -> PIN));

    else
        ACCESS_REG((CONFIG_Ptr -> PORT), GPIOODR) |= ( 1 << (CONFIG_Ptr -> PIN));

    return;
}


/******************************************************************************
 *
 * Function Name: GPIO_initClock
 *
 * Description: A function that's responsible for starting the clock of
 *              a given Port.
 *
 * Input: uint32_t PORT
 * Output: void
 *
 *****************************************************************************/
void GPIO_initClock (uint32_t PORT)
{
    switch (PORT)
    {
        case PORTA:
            RCGCGPIO |= (1<<PIN_0);
            break;
        case PORTB:
            RCGCGPIO |= (1<<PIN_1);
            break;
        case PORTC:
            RCGCGPIO |= (1<<PIN_2);
            break;
        case PORTD:
            RCGCGPIO |= (1<<PIN_3);
            break;
        case PORTE:
            RCGCGPIO |= (1<<PIN_4);
            break;
        case PORTF:
            RCGCGPIO |= (1<<PIN_5);
            break;
        default:
    }

    return;
}
