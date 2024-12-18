#include "stm32f0xx.h"
#include "led.h"
#include "oled.h"


void led_init(void) {
    //We are using PA 8-10
    RCC->AHBENR |= RCC_AHBENR_GPIOAEN;
    RCC->APB2ENR |= RCC_APB2ENR_TIM1EN;
    GPIOA->MODER &= ~(0x00FF0000);
    GPIOA->MODER |= 0x00AA0000;

    GPIOA->AFR[1] &= ~(0xF << (4 * 0)); // Clear PA8
    GPIOA->AFR[1] &= ~(0xF << (4 * 1)); // Clear PA9
    GPIOA->AFR[1] &= ~(0xF << (4 * 2)); // Clear PA10

    GPIOA->AFR[1] |= (0x2 << (4 * 0)); // AF2 for PA8 (TIM1_CH1)
    GPIOA->AFR[1] |= (0x2 << (4 * 1)); // AF2 for PA9 (TIM1_CH2)
    GPIOA->AFR[1] |= (0x2 << (4 * 2)); // AF2 for PA10 (TIM1_CH3)

    TIM1->BDTR |= 0x8000;

    TIM1->PSC = 47;    
    TIM1->ARR = 1000 - 1;   
    TIM1->CCMR1 |= (6 << TIM_CCMR1_OC1M_Pos) | (6 << TIM_CCMR1_OC2M_Pos); // PWM mode 1 for CH1, CH2
    TIM1->CCMR2 |= (6 << TIM_CCMR2_OC3M_Pos); // PWM mode 1 for CH3
    TIM1->CCER |= 0x1111;
    TIM1->CCMR2 |= 0x0800;
    TIM1->CR1 |= TIM_CR1_CEN;
}

void set_color(uint32_t red, uint32_t green, uint32_t blue) {
    TIM1 -> CCR1 = red;
    TIM1 -> CCR2 = green;
    TIM1 -> CCR3 = blue;
}

void led_off(void) {
    //Security system is off, LED is off
    set_color(999, 999, 999);
}

void white_pulse(void) {
    set_color(1, 1, 1);
}

void red_flash(void) {
    //Security system is triggered, curr_state is set to 1 and flashes red until
    //security is disabled, sending curr_state to 0 ending flashing
    uint64_t initial_delay1 = 1000000000;  
    int flashes = 20; // Choose a number of flashes for smooth transition
    uint64_t current_delay1 = initial_delay1;

    for (int i = 0; i < flashes; i++) {
        // Turn the LED ON
        set_color(1, 999, 999);
        nano_wait(current_delay1);
        led_off();       
        nano_wait(current_delay1);
    }

    set_color(1, 999, 999);
}

void green(void) {
    set_color(999, 999, 1); //Passcode is input correctly, LED turns green
}

void green_flash(void)
{
    uint64_t initial_delay = 4500000000;  
    uint64_t step; // Step to reduce delay

    int flashes = 15; // Choose a number of flashes for smooth transition
    step = 500000000;

    uint64_t current_delay = initial_delay;

    for (int i = 0; i < flashes; i++) {
        // Turn the LED ON
        green();
        nano_wait(current_delay);
        green();       
        nano_wait(current_delay);   

        current_delay = current_delay - step;
    }
    white_pulse(); //After 15 seconds, light will become solid state of green to show security system is now set
}

void led_main(int system_state) {
    //system_state at 0 means security is off
    if(system_state == 0) //security off
    {
        led_off();
    }
    else if(system_state == 1) //passcode correctly input, light flashes to show user security will turn on shortly
    {
        green_flash();
    }  
    else if(system_state == 2) //security system on
    {   
        white_pulse();
    }
    else if(system_state == 3) //security system triggered
    {
        red_flash();
    }
}