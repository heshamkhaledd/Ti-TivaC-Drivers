/******************************************************************************
 *
 * Module: UART0
 *
 * File Name: uart0.c
 *
 * Description: Source file for the uart0
 *
 * Engineer: Hesham Khaled
 *
 *******************************************************************************/

#include "uart0.h"

void UART0_init(void)
{
    /* Enable UART0 Clock */
    RCGCUART |= (1<<PIN_0);

    /* PA0, PA1 Configuration */
    RCGCGPIO |= (1<<PIN_0); /* Enable PORTA Clock */
    ACCESS_REG(PORTA,GPIOLOCK) = 0x4C4F434B;  /* Unlock PORTA */
    ACCESS_REG(PORTA,GPIOCR) |= ((1<<PIN_0) | (1<<PIN_1)); /* Enable editing in PIN0/PIN1 */
    ACCESS_REG(PORTA,GPIODEN)  |= (1<<PIN_0) | (1<<PIN_1); /* Enable Digital Behaviour */

    CLEAR_BIT(UART0,UARTCTL,PIN_0); /* Disable UART0 For editing */

    /* Enable special function to the pins */
    ACCESS_REG(PORTA,GPIOAFSEL) |= (1<<PIN_0) | (1<<PIN_1);
    ACCESS_REG(PORTA,GPIOPCTL)  |= (1<<PIN_0) | (1<<PIN_4);

    /* Baud rate of 9600 */
    ACCESS_REG(UART0,UARTIBRD) = 104;
    ACCESS_REG(UART0,UARTFBRD) = 11;

    /* 8 Bit word size , Fifo enabled */
    SET_BIT(UART0,UARTLCRH,PIN_4);
    SET_BIT(UART0,UARTLCRH,PIN_5);
    SET_BIT(UART0,UARTLCRH,PIN_6);

    /* Choose the Internal CPU clock for UART */
    ACCESS_REG(UART0,UARTCC) = 0x0;

    /* Enable Transmitting and Receiving */
    ACCESS_REG(UART0,UARTCTL) |= (1<<PIN_8) | (1<<PIN_9);

    /*Re-enable UART0 again */
    SET_BIT(UART0,UARTCTL,PIN_0);
}



void UART0_sendByte (uint8_t c)
{
    while (BIT_IS_SET(UART0,UARTFR,PIN_5));
    ACCESS_REG(UART0,UARTDR) = c;
}

void UART0_sendString (uint8_t *str)
{
    while (*str)
    {
        UART0_sendByte(*(str++));
    }
}

uint8_t UART0_receiveByte (void)
{
    uint8_t byte;
    while(BIT_IS_SET(UART0,UARTFR,PIN_4));
    byte = ACCESS_REG(UART0,UARTDR);
    return byte;
}
