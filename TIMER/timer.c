/******************************************************************************
 *
 * Module: TIMER
 *
 * File Name: timer.c
 *
 * Description: Source file for the timer driver
 *
 * Engineer: Hesham Khaled
 *
 *******************************************************************************/

#include "gpio.h"
#include "timer.h"


#if (SysTick_EN == 1)

void SysTick_init (SysTick_CONFIG const *CONFIG_Ptr)
{

    /* Enable the Counter */
    SET_BIT (SysTick,STCTRL,PIN_0);

    /* Choose the Interrupt state when the count down is over */
    if (CONFIG_Ptr -> state == INTERRUPT_ON)
        SET_BIT(SysTick,STCTRL,1);
    else if (CONFIG_Ptr -> state == INTERRUPT_OFF)
        CLEAR_BIT(SysTick,STCTRL,1);

    /* Determine the Clock source for the SysTick count down */
    if (CONFIG_Ptr -> source == Sys_CLOCK)
        SET_BIT(SysTick,STCTRL,2);
    else if (CONFIG_Ptr -> source == OSCILLATOR)
        CLEAR_BIT(SysTick,STCTRL,2);

    /* Set the initial value for the counter */
    if ((CONFIG_Ptr -> initial_value) > 0xFFFFFF)
    {
        while (1) {

        }
    }

    ACCESS_REG(SysTick,STRELOAD) = CONFIG_Ptr -> initial_value;

    sei() ;

    return;
}

void SysTick_fun (void)
{

}

#endif


#if (TIMER_32_16_EN == 1)
void TIMER_32_16_init (TIMER_32_16_CONFIG const *CONFIG_Ptr)
{
    
    TIMER_32_16_initClock (CONFIG_Ptr -> timer);

    if (CONFIG_Ptr -> bit_mode == MODE_32)
        ACCESS_REG((CONFIG_Ptr -> timer), GPTMCFG ) = 0xFFF8;

    else if (CONFIG_Ptr -> bit_mode == MODE_16)
    {
        ACCESS_REG((CONFIG_Ptr -> timer), GPTMCFG ) = (ACCESS_REG((CONFIG_Ptr -> timer), GPTMCFG) & 0xFFFB) | (MODE_16 & 0x0004);
        if (CONFIG_Ptr -> channel == 'A')
            ACCESS_REG((CONFIG_Ptr -> timer), GPTMTAILR ) = 0xFFFF;

        else if (CONFIG_Ptr -> channel == 'B')
            ACCESS_REG((CONFIG_Ptr -> timer), GPTMTBILR ) = 0xFFFF;
    }
    
    else if (CONFIG_Ptr -> bit_mode == MODE_RTC)
        ACCESS_REG((CONFIG_Ptr -> timer), GPTMCFG ) = (ACCESS_REG((CONFIG_Ptr -> timer), GPTMCFG) & 0xFFF7) | (MODE_RTC & 0x0001);

    if (CONFIG_Ptr -> operation_mode == ONE_SHOT)
    {

        if (CONFIG_Ptr -> channel == 'A')
        {
            if (CONFIG_Ptr -> operation_mode_2 == COMPARE)
            {
                CLEAR_BIT(CONFIG_Ptr -> timer,GPTMTAMR,PIN_3);
                ACCESS_REG(CONFIG_Ptr -> timer,GPTMTAMATCHR) = CONFIG_Ptr -> compare_value;
            }
            else if (CONFIG_Ptr -> operation_mode_2 == OVF)
            {
                    ACCESS_REG(CONFIG_Ptr -> timer,GPTMTAMATCHR) = 0xFFFF;
            }

            SET_BIT(CONFIG_Ptr -> timer,GPTMTAMR,PIN_0);
            CLEAR_BIT(CONFIG_Ptr -> timer,GPTMTAMR,PIN_1);
        }
        else if (CONFIG_Ptr -> channel == 'B')
        {
            if (CONFIG_Ptr -> operation_mode_2 == COMPARE)
            {
                CLEAR_BIT(CONFIG_Ptr -> timer,GPTMTBMR,PIN_3);
                ACCESS_REG(CONFIG_Ptr -> timer,GPTMTBMATCHR) = CONFIG_Ptr -> compare_value;
            }

            else if (CONFIG_Ptr -> operation_mode_2 == OVF)
            {
                ACCESS_REG(CONFIG_Ptr -> timer,GPTMTBMATCHR) = 0xFFFF;
            }

            SET_BIT(CONFIG_Ptr -> timer,GPTMTBMR,PIN_0);
            CLEAR_BIT(CONFIG_Ptr -> timer,GPTMTBMR,PIN_1);
        }
    }
    else if (CONFIG_Ptr -> operation_mode == PERIODIC)
    {
        if (CONFIG_Ptr -> channel == 'A')
        {
            if (CONFIG_Ptr -> operation_mode_2 == COMPARE)
            {
                CLEAR_BIT(CONFIG_Ptr -> timer,GPTMTAMR,PIN_3);
                ACCESS_REG(CONFIG_Ptr -> timer,GPTMTAMATCHR) = CONFIG_Ptr -> compare_value;
            }
            else if (CONFIG_Ptr -> operation_mode_2 == OVF)
            {
                    ACCESS_REG(CONFIG_Ptr -> timer,GPTMTAMATCHR) = 0xFFFF;
            }

            SET_BIT(CONFIG_Ptr -> timer,GPTMTAMR,PIN_1);
            CLEAR_BIT(CONFIG_Ptr -> timer,GPTMTAMR,PIN_0);
        }
        else if (CONFIG_Ptr -> channel == 'B')
        {
            if (CONFIG_Ptr -> operation_mode_2 == COMPARE)
            {
                CLEAR_BIT(CONFIG_Ptr -> timer,GPTMTBMR,PIN_3);
                ACCESS_REG(CONFIG_Ptr -> timer,GPTMTBMATCHR) = CONFIG_Ptr -> compare_value;
            }

            else if (CONFIG_Ptr -> operation_mode_2 == OVF)
            {
                ACCESS_REG(CONFIG_Ptr -> timer,GPTMTBMATCHR) = 0xFFFF;
            }

            SET_BIT(CONFIG_Ptr -> timer,GPTMTBMR,PIN_1);
            CLEAR_BIT(CONFIG_Ptr -> timer,GPTMTBMR,PIN_0);
        }
    }
    else if (CONFIG_Ptr -> operation_mode == CAPTURE)
    {
        if (CONFIG_Ptr -> channel == 'A')
        {
            CLEAR_BIT(CONFIG_Ptr -> timer,GPTMTAMR,PIN_3);
            SET_BIT(CONFIG_Ptr -> timer, GPTMTAMR,PIN_0);
            SET_BIT(CONFIG_Ptr -> timer, GPTMTAMR,PIN_1);

                        
            if (CONFIG_Ptr ->signal_mode == EDGE_COUNT)
            {
                CLEAR_BIT(CONFIG_Ptr -> timer,GPTMTAMR,PIN_2);
            }
            else if (CONFIG_Ptr -> signal_mode == EDGE_TIME)
            {
                SET_BIT(CONFIG_Ptr -> timer,GPTMTAMR,PIN_2);
            }
        }
        else if (CONFIG_Ptr -> channel == 'B')
        {
            CLEAR_BIT(CONFIG_Ptr -> timer,GPTMTBMR,PIN_3);
            SET_BIT(CONFIG_Ptr -> timer, GPTMTBMR,PIN_0);
            SET_BIT(CONFIG_Ptr -> timer, GPTMTBMR,PIN_1);

            if (CONFIG_Ptr ->signal_mode == EDGE_COUNT)
            {
                CLEAR_BIT(CONFIG_Ptr -> timer,GPTMTAMR,PIN_2);
            }
            else if (CONFIG_Ptr -> signal_mode == EDGE_TIME)
            {
                SET_BIT(CONFIG_Ptr -> timer,GPTMTAMR,PIN_2);
            }
        }
    }

    if (CONFIG_Ptr -> count_mode == COUNT_DOWN)
    {
        if (CONFIG_Ptr -> channel == 'A')
            CLEAR_BIT (CONFIG_Ptr -> timer,GPTMTAMR,PIN_4);
        else if (CONFIG_Ptr -> channel == 'B')
            CLEAR_BIT(CONFIG_Ptr -> timer, GPTMTBMR, PIN_4);
    }
    else if (CONFIG_Ptr -> count_mode == COUNT_UP)
    {
       if (CONFIG_Ptr -> channel == 'A')
            SET_BIT (CONFIG_Ptr -> timer,GPTMTAMR,PIN_4);
        else if (CONFIG_Ptr -> channel == 'B')
            SET_BIT (CONFIG_Ptr -> timer,GPTMTBMR,PIN_4);
    }

    if (CONFIG_Ptr -> interrupt_state == INTT_ON)
    {
        if (CONFIG_Ptr -> channel == 'A')
            SET_BIT(CONFIG_Ptr ->timer,GPTMTAMR,PIN_5);
        else if (CONFIG_Ptr -> channel == 'B');
            SET_BIT(CONFIG_Ptr -> timer, GPTMTBMR,PIN_5);

            TIMER_32_16_initInterrupt (CONFIG_Ptr);
    }
    else if (CONFIG_Ptr -> interrupt_state == INTT_OFF)
    {
        if (CONFIG_Ptr -> channel == 'A')
            CLEAR_BIT(CONFIG_Ptr ->timer,GPTMTAMR,PIN_5);
        else if (CONFIG_Ptr -> channel == 'B');
            CLEAR_BIT(CONFIG_Ptr -> timer, GPTMTBMR,PIN_5);
    }

    if (CONFIG_Ptr -> start_mode == ON_ENABLE)
    {
        if (CONFIG_Ptr -> channel == 'A')
            CLEAR_BIT(CONFIG_Ptr -> timer, GPTMTAMR, PIN_6);
        else if (CONFIG_Ptr -> channel == 'B')
            CLEAR_BIT(CONFIG_Ptr -> timer, GPTMTBMR,PIN_6);
    }
    else if (CONFIG_Ptr -> start_mode == ON_TRIGGER)
    {
        if (CONFIG_Ptr -> channel == 'A')
            SET_BIT(CONFIG_Ptr -> timer, GPTMTAMR, PIN_6);
        else if (CONFIG_Ptr -> channel == 'B')
            SET_BIT(CONFIG_Ptr -> timer, GPTMTBMR,PIN_6);
    }

    if (CONFIG_Ptr -> channel == 'A')
    {
        ACCESS_REG(CONFIG_Ptr -> timer, GPTMTAPR) = CONFIG_Ptr -> prescaler;
    }
    else if (CONFIG_Ptr -> channel == 'B')
    {
        ACCESS_REG(CONFIG_Ptr -> timer, GPTMTBPR) = CONFIG_Ptr -> prescaler;
    }

    SET_BIT(CONFIG_Ptr -> timer, GPTMCTL, 0);
}


void TIMER_32_16_initClock (uint32_t timer)
{
    switch (timer)
    {
        case TIMER0:
            SET_BIT(TIMER0,RCGCTIMER,PIN_0);
            CLEAR_BIT(TIMER0,GPTMCTL,0);
            break;
        
        case TIMER1:
            SET_BIT(TIMER1,RCGCTIMER,PIN_1);
            CLEAR_BIT(TIMER0,GPTMCTL,0);
            break;

        case TIMER2:
            SET_BIT(TIMER2,RCGCTIMER,PIN_2);
            CLEAR_BIT(TIMER0,GPTMCTL,0);
            break;

        case TIMER3:
            SET_BIT(TIMER3,RCGCTIMER,PIN_3);
            CLEAR_BIT(TIMER0,GPTMCTL,0);
            break;

        case TIMER4:
            SET_BIT(TIMER4,RCGCTIMER,PIN_4);
            CLEAR_BIT(TIMER0,GPTMCTL,0);
            break;
        
        case TIMER5:
            SET_BIT(TIMER5,RCGCTIMER,PIN_5);
            CLEAR_BIT(TIMER0,GPTMCTL,0);
            break;
    }

    return;
}


void TIMER_32_16_initInterrupt (TIMER_32_16_CONFIG const *CONFIG_Ptr)
{
    if (CONFIG_Ptr -> channel == 'A')
    {
        if (CONFIG_Ptr -> operation_mode_2 == OVF)
            SET_BIT(CONFIG_Ptr -> timer,GPTMIMR,PIN_0);
        else if (CONFIG_Ptr -> bit_mode == MODE_RTC)
            SET_BIT(CONFIG_Ptr -> timer,GPTMIMR,PIN_3);
        else if (CONFIG_Ptr -> operation_mode_2 == COMPARE)
            SET_BIT(CONFIG_Ptr -> timer,GPTMIMR,PIN_4);
    }

    else if(CONFIG_Ptr -> channel == 'B')
    {
        if (CONFIG_Ptr -> operation_mode_2 == OVF)
            SET_BIT(CONFIG_Ptr -> timer,GPTMIMR,PIN_8);
        else if (CONFIG_Ptr -> operation_mode_2 == COMPARE)
            SET_BIT(CONFIG_Ptr -> timer,GPTMIMR,PIN_11);
    }
}

void TIMER_32_16_fun (void)
{
    static uint8_t flag = 0;
    flag ++;
    if (flag == 2)
    {
        CLEAR_BIT(PORTF,GPIODATA,PIN_3);
        CLEAR_BIT(PORTF,GPIODATA,PIN_2);
        flag = 0;
    }
    SET_BIT(TIMER0,GPTMICR,PIN_4);
}

#endif



