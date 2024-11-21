// MOTION SENSOR INITIALIZATION AND FUNCTIONS
#include "stm32f0xx.h"
#include <stdint.h>
#include "sensor.h"
#include "alarm.h"
#include "oled.h"

void internal_clock();

//global variables
uint8_t hist; // 8 sample bits of input
int motion_cnt = 0;
int no_motion_ct = 0;

void enable_sensor_ports(){
    // RCC for GPIOA
    RCC -> AHBENR |= RCC_AHBENR_GPIOAEN;

    //PA6 for output & PA0 for input; PA3 is output (buzzer)
    GPIOA -> MODER &= ~(GPIO_MODER_MODER6 | GPIO_MODER_MODER0);
    GPIOA -> MODER |= GPIO_MODER_MODER6_0; // | GPIO_MODER_MODER3_0;
    GPIOA -> PUPDR |= GPIO_PUPDR_PUPDR0_1; // pull down resistor added
}
void enable_sensor(){
    //turning on pin 6
    GPIOA -> BSRR |= GPIO_BSRR_BS_6;
}

void disable_sensor(){
    //turning off PA6
    GPIOA -> BRR |= GPIO_BRR_BR_6;
}

void read_motion() {
    if (hist >= 0x01){ //0x00000001
        // motion_cnt++;
        alarm();
        clear_display();
        spi1_display1("Motion Detected"); //when motion is detected display on OLED
        motion_cnt = 0; // resets motion counter
    }
    if (hist <= 0xfe) { // 0x11111110
        clear_display();
        spi1_display1("No Motion Detected"); //when motion is detected display on OLED
        no_motion_ct++;
    }
    
    // if (motion_cnt >= 7) {
    //     alarm();
    //     clear_display();
    //     spi1_display1("Motion Detected"); //when motion is detected display on OLED
    //     motion_cnt = 0; // resets motion counter
    // }
    // else {
    //     GPIOA -> BSRR |= GPIO_BSRR_BR_3;
    //     no_motion_ct = 0;
    // }
}

void update_hist_sensor(){
    int temp = GPIOA -> IDR & 1;
    hist = (hist << 1) | temp;
}

void TIM6_DAC_IRQHandler() {
    TIM6 ->  SR &= ~TIM_SR_UIF;
    update_hist_sensor();
    read_motion();
}

void init_tim6(void) {
    RCC-> APB1ENR |= RCC_APB1ENR_TIM7EN;
    TIM6 -> PSC = 4800 - 1;
    TIM6-> ARR = 10 - 1;
    TIM6 -> DIER |= TIM_DIER_UIE;
    NVIC -> ISER[0] = 1 << TIM6_DAC_IRQn;
    TIM6 -> CR1 |= TIM_CR1_CEN;
}
