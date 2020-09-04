/******************************************************************************
 *
 * Module: UART0
 *
 * File Name: uart0.h
 *
 * Description: Header file for the uart0
 *
 * Engineer: Hesham Khaled
 *
 *******************************************************************************/

#ifndef UART0_H_
#define UART0_H_

#include "std_types.h"
#include "common_macros.h"
#include "gpio.h"

#define UART0       0x4000C000
#define RCGCUART     (*((volatile uint32_t*)(0x400FE000+0x618)))


/* Integer divisor */
#define UARTIBRD    0x024

/* Fraction divisor. *64(fraction)+0.5 */
#define UARTFBRD    0x028



/* D0: Enable
 * D5: High speed enable
 * D8: Receive Enable
 * D9: Transmit Enable
 */
#define UARTCTL     0x030

/* D3: Enable 2 stop bits
 * D4: FIFO Enable
 * D5/6: 1/1 to make word size = 8 bits
 */
#define UARTLCRH    0x02C

/* Lower 8-Bits: Data Sent or Received */
#define UARTDR      0x000

/* UART Status Register */
#define UARTFR      0x018

/* Clock select Register */
#define UARTCC      0xFC8



typedef enum{
    POLLING,INTERRUPT
}UART0_Mode;


void UART0_init (void);
void UART0_sendByte (uint8_t c);
void UART0_sendString (uint8_t *str);
uint8_t UART0_receiveByte (void);

#endif /* UART0_H_ */
