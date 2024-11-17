// ALARM FUNCTIONS
#include "stm32f0xx.h"

void enable_alarm_ports();
void alarm(void);

void enable_alarm_ports(){
    //RCC for GPIOA
    RCC -> AHBENR |= RCC_AHBENR_GPIOAEN;

    GPIOA -> MODER &= ~(GPIO_MODER_MODER3);
    GPIOA -> MODER |= GPIO_MODER_MODER3_0; // PA3 as output
}
void alarm(void){
    clear_display();
    spi1_display1("ALARMMM");
    GPIOA -> BSRR |= GPIO_BSRR_BS_3;
    nano_wait(5000000);
    GPIOA -> BSRR |= GPIO_BSRR_BR_3;
}