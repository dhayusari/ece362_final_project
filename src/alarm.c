// ALARM FUNCTIONS
#include "stm32f0xx.h"
#include "oled.h"
#include "alarm.h"
#include "led.h"

void enable_alarm_ports(){
    //RCC for GPIOA
    RCC -> AHBENR |= RCC_AHBENR_GPIOAEN;

    GPIOA -> MODER &= ~(GPIO_MODER_MODER3);
    GPIOA -> MODER |= GPIO_MODER_MODER3_0; // PA3 as output
};

void alarm(void){
    clear_display();
    spi1_display1("ALARMMM");
    GPIOA -> BSRR |= GPIO_BSRR_BS_3;
    //led_main(3);
    // nano_wait(50000000);
    // GPIOA -> BSRR |= GPIO_BSRR_BR_3;
}

void disable_alarm(void) {
    GPIOA -> BSRR |= GPIO_BSRR_BR_3;
    //led_main(0);
}