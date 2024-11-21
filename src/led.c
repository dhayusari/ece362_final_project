#include "stm32f0xx.h"

void nano_wait(unsigned int n);
void setup_tim1(void);
void set_color(uint16_t red, uint16_t green, uint16_t blue);
void led_off(void);
void white_pulse(void);
void red_flash(int curr_state);
void green(void);
void green_flash(void);
void led_main(int system_state);

void setup_tim1(void) {
    //We are using PA 8-10
    RCC->AHBENR |= RCC_AHBENR_GPIOAEN;
    RCC->APB2ENR |= RCC_APB2ENR_TIM1EN;
    GPIOA->MODER &= ~(0x00FF0000);
    GPIOA->MODER |= 0x00AA0000;

    GPIOA->AFR[1] &= ~(0xf<<(4*(0)));
    GPIOA->AFR[1] &= ~(0xf<<(4*(1)));
    GPIOA->AFR[1] &= ~(0xf<<(4*(2)));
    GPIOA->AFR[1] &= ~(0xf<<(4*(3)));

    GPIOA->AFR[1] |= (0x2<<(4*(0)));
    GPIOA->AFR[1] |= (0x2<<(4*(1)));
    GPIOA->AFR[1] |= (0x2<<(4*(2)));
    GPIOA->AFR[1] |= (0x2<<(4*(3)));

    TIM1->BDTR |= 0x8000;

    TIM1->PSC = 1-1;
    TIM1->ARR = 2400-1;
    TIM1->CCMR1 &= ~(0x1010);
    TIM1->CCMR2 &= ~(0x1010);
    TIM1->CCMR1 |= 0x6060;
    TIM1->CCMR2 |= 0x6060;
    TIM1->CCER |= 0x1111;
    TIM1->CCMR2 |= 0x0800;
    TIM1->CR1 |= TIM_CR1_CEN;
}

void set_color(uint16_t red, uint16_t green, uint16_t blue) {
    TIM1 -> CCR1 = red;
    TIM1 -> CCR2 = green;
    TIM1 -> CCR3 = blue;
}

void led_off(void) {
    //Security system is off, LED is off
    set_color(0, 0, 0);
}

void white_pulse(void) {
    //Pulses LED in white color, showing that the security system is on and enabled
    for(uint16_t i = 0; i < 65535; i += 1000) {
        set_color(i, i, i);
        nano_wait(10000);
    }
    for(uint16_t i = 65535; i > 0; i -= 1000) {
        set_color(i, i, i);
        nano_wait(10000);
    }
}

void red_flash(int curr_state) {
    //Security system is triggered, curr_state is set to 1 and flashes red until
    //security is disabled, sending curr_state to 0 ending flashing
    while(curr_state == 3) {
        set_color(65535, 0, 0);
        nano_wait(20000);
        set_color(0, 0, 0);
        nano_wait(20000);
    }
}

void green(void) {
    set_color(0, 65535, 0); //Passcode is input correctly, LED turns green
}

void green_flash(void)
{
    uint32_t initialDelay = 500000; //Initial delay speed
    uint32_t minDelay = 100000; //Fastest delay speed
    uint32_t step = (initialDelay - minDelay) / 15; //Amount to reduce the delay for each second

    for (int i = 0; i < 15; i++) 
    {
        green(); 
        nano_wait(initialDelay);
        set_color(0, 0, 0);
        nano_wait(initialDelay);

        //Decrease delay to increase flashing speed
        if (initialDelay > minDelay)
            initialDelay -= step;
    }

    green(); //After 15 seconds, light will become solid state of green to show security system is now set
}

void led_main(int system_state) {
    setup_tim1();

    //system_state at 0 means security is off
    while(1) {
        if(system_state == 0) //security off
            led_off();
        else if(system_state == 1) //passcode correctly input, light flashes to show user security will turn on shortly
            green_flash();
        else if(system_state == 2) //security system on
            white_pulse();
        else if(system_state == 3) //security system triggered
            red_flash(system_state);
    }
}