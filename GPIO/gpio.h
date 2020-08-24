/******************************************************************************
 *
 * Module: GPIO
 *
 * File Name: gpio.h
 *
 * Description: Header file for the GPIO driver
 *
 * Engineer: Hesham Khaled
 *
 *******************************************************************************/
#ifndef GPIO_H_
#define GPIO_H_



/*******************************************************************************
 *                          GPIO Common Macros
 ******************************************************************************/

#define ACCESS_REG(BASE,OFFSET) (*((volatile uint32_t*)(BASE+OFFSET)))


/*******************************************************************************
 *                   GPIO Registers and Ports Definitions
 ******************************************************************************/

                                /* Ports Base Address */
#define PORTA     0x40004000
#define PORTB     0x40005000
#define PORTC     0x40006000
#define PORTD     0x40007000
#define PORTE     0x40024000
#define PORTF     0x40025000


                                /* Registers offset */

/* GPIODATA: Read/Write Register */
#define GPIODATA    0x3FC

/* GPIODIR: Direction Register
 * 0 := Input
 * 1 := Output
 */
#define GPIODIR     0x400

/* GPIODEN: Digital Behaviour
 * 0 := Digital Behaviour OFF
 * 1 := Digital Behaviour ON
 */
#define GPIODEN     0x51C

/* GPIOAMSEL: Analog Behaviour
 * 0 := Analog Behaviour OFF
 * 1 := Analog Behaviour ON
 */
#define GPIOAMSEL   0x528

/* GPIOUR: Pull-up Resistor
 * 0 := OFF
 * 1 := ON
 */
#define GPIOUR      0x510

/* GPIODR: Pull-Down Resistor
 * 0 := OFF
 * 1 := ON
 */
#define GPIOPDR     0x514

/* GPIOODR: Open-Drain Enable
 * 0 := Disabled
 * 1 := Enabled
 */
#define GPIOODR     0x50C


/* Registers Memory Mapped Access */


/* RCGCGPIO */
#define RCGCGPIO     (*((volatile uint32_t*)(0x400FE000+0x608)))

/* GPIODATA */
#define GPIODATAA    (*((volatile uint32_t*)(PORTA+GPIODATA)))
#define GPIODATAB    (*((volatile uint32_t*)(PORTB+GPIODATA)))
#define GPIODATAC    (*((volatile uint32_t*)(PORTC+GPIODATA)))
#define GPIODATAD    (*((volatile uint32_t*)(PORTD+GPIODATA)))
#define GPIODATAE    (*((volatile uint32_t*)(PORTE+GPIODATA)))
#define GPIODATAF    (*((volatile uint32_t*)(PORTF+GPIODATA)))


/* GPIODIR */
#define GPIODIRA    (*((volatile uint32_t*)(PORTA+GPIODIR)))
#define GPIODIRB    (*((volatile uint32_t*)(PORTB+GPIODIR)))
#define GPIODIRC    (*((volatile uint32_t*)(PORTC+GPIODIR)))
#define GPIODIRD    (*((volatile uint32_t*)(PORTD+GPIODIR)))
#define GPIODIRE    (*((volatile uint32_t*)(PORTE+GPIODIR)))
#define GPIODIRF    (*((volatile uint32_t*)(PORTF+GPIODIR)))

/* GPIODEN */
#define GPIODENA    (*((volatile uint32_t*)(PORTA+GPIODENA)))
#define GPIODENB    (*((volatile uint32_t*)(PORTB+GPIODENB)))
#define GPIODENC    (*((volatile uint32_t*)(PORTC+GPIODENC)))
#define GPIODEND    (*((volatile uint32_t*)(PORTD+GPIODEND)))
#define GPIODENE    (*((volatile uint32_t*)(PORTE+GPIODENE)))
#define GPIODENF    (*((volatile uint32_t*)(PORTF+GPIODENF)))

/* GPIOUR */
#define GPIOURA    (*((volatile uint32_t*)(PORTA+GPIOURA)))
#define GPIOURB    (*((volatile uint32_t*)(PORTB+GPIOURB)))
#define GPIOURC    (*((volatile uint32_t*)(PORTC+GPIOURC)))
#define GPIOURD    (*((volatile uint32_t*)(PORTD+GPIOURD)))
#define GPIOURE    (*((volatile uint32_t*)(PORTE+GPIOURE)))
#define GPIOURF    (*((volatile uint32_t*)(PORTF+GPIOURF)))



                    /* PIN Definitions */
#define PIN_0       0
#define PIN_1       1
#define PIN_2       2
#define PIN_3       3
#define PIN_4       4
#define PIN_5       5
#define PIN_6       6
#define PIN_7       7





/*******************************************************************************
 *                  Configuration Structures and Enumerators
 ******************************************************************************/


                                /* Enumerators */

/******************************************************************************
 * Name: PIN_DIR
 * Description: Enumerator to hold the type of the required PIN Direction
 * 0 := Input
 * 1 := Output
 *****************************************************************************/
typedef enum{

    INPUT, OUTPUT

}PIN_DIR;


/******************************************************************************
 * Name: PIN_LOGIC
 * Description: Enumerator to hold the type of the required LOGIC
 * 0 := Pull Up Resistor
 * 1 := Pull Down Resistor
 * 2 := Open Drain (Neither Nor)
 *****************************************************************************/
typedef enum{

    PULL_UP, PULL_DOWN, OPEN_DRAIN

}PIN_LOGIC;


/******************************************************************************
 * Name: PIN_LOGIC
 * Description: Enumerator to hold the type of the required LOGIC
 * 0 := Digital
 * 1 := Analog
 *****************************************************************************/
typedef enum{
    DIGITAL, ANALOG
}PIN_BEHAVIOUR;


                                    /* Structs */

/******************************************************************************
 * Name: PIN_CONFIG
 * Description: Struct to hold the Configuration of the required Port and Pin
 * PORT / REG / PIN / DIRECTION / LOGIC
 *****************************************************************************/
typedef struct{

    uint32_t        PORT;
    uint32_t        PIN;
    PIN_BEHAVIOUR   BEHAVIOUR;
    PIN_DIR         DIRECTION;
    PIN_LOGIC       LOGIC;
}PIN_CONFIG;



/*******************************************************************************
 *                          Function Definitions
 ******************************************************************************/

void GPIO_configurePIN (PIN_CONFIG const *CONFIG_Ptr);
void GPIO_initClock (uint32_t PORT);

#endif /* GPIO_H_ */
