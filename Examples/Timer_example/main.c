#include "gpio.h"
#include "timer.h"

PIN_CONFIG first_instance  = {PORTF,PIN_1,DIGITAL,OUTPUT,PULL_UP,INTERRUPT_OFF};
PIN_CONFIG second_instance = {PORTF,PIN_2,DIGITAL,OUTPUT,PULL_UP,INTERRUPT_OFF};
PIN_CONFIG third_instance  = {PORTF,PIN_3,DIGITAL,OUTPUT,PULL_UP,INTERRUPT_OFF};




int main(void)
{
    GPIO_configurePIN(&first_instance);
    GPIO_configurePIN(&second_instance);
    GPIO_configurePIN(&third_instance);
    TIMER_32_16_CONFIG timer_instance = {TIMER0,'A',MODE_32,PERIODIC,COMPARE};
    timer_instance.count_mode = COUNT_UP;
    timer_instance.interrupt_state = INT_ON;
    timer_instance.start_mode = ON_ENABLE;
    timer_instance.compare_value = 16000000;
    TIMER_32_16_init(&timer_instance);

   while (1)
   {
   }
}
