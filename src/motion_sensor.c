// MOTION SENSOR INITIALIZATION AND FUNCTIONS
#include "stm32f0xx.h"
#include <stdint.h>
#include "keypad.h"
#include "oled.h"
#include "sensor.h"
#include "led.h"
#include "alarm.h"

uint8_t hist_sensor; // 8 sample bits of input
int motion_cnt = 0;
int no_motion_ct = 0;
int motion = 1;
int state[2] = {0, 0};
int detection = 0;

void enable_sensor_ports(){
    // RCC for GPIOA
    RCC -> AHBENR |= RCC_AHBENR_GPIOAEN;

    //PA6 for output & PA0 for input; PA3 is output (buzzer)
    GPIOA -> MODER &= ~(GPIO_MODER_MODER6 | GPIO_MODER_MODER0 | GPIO_MODER_MODER3);
    GPIOA -> MODER |= GPIO_MODER_MODER6_0 | GPIO_MODER_MODER3_0;
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
    // char key = get_keypress();
    if (hist_sensor & 0xFF) {  // Check if the latest bit indicates motion
        motion_cnt++;
        no_motion_ct = 0;  // Reset no motion counter
        if (motion_cnt >= 255) {  // Threshold for alarm
            motion_cnt = 0;  // Reset motion counter
            state[0] = state[1];
            state[1] = 1;
            //TIM6 -> CR1 &= ~(TIM_CR1_CEN);
        } 
    } else {  // No motion detected
        no_motion_ct++;
        
        if (no_motion_ct >= 8) {  // Reset after consecutive no-motion states
            motion_cnt = 0;
            state[0] = state[1];
            state[1] = 0;
        }
        //return 0;
    }

    if (state[0] == state[1]) {
        if (state[1] == 1) {
            if (detection > 7) {
                alarm();
                clear_display();
                spi1_display1("Motion Detected!");
                TIM6 -> CR1 &= ~(TIM_CR1_CEN);
            }
            else {
                detection++;
            }
        } else {
            clear_display();
            spi1_display1("No Motion!");
            }
    }
    
}

// void read_motion() {
//     if (hist == 0x01){ //0x00000001
//         // motion_cnt++;
//         alarm();
//         clear_display();
//         spi1_display1("Motion Detected"); //when motion is detected display on OLED
//         //motion_cnt = 0; // resets motion counter
//     }
//     if (hist == 0xfe) { // 0x11111110
//         clear_display();
//         spi1_display1("No Motion Detected"); //when motion is detected display on OLED
//        // no_motion_ct++;
//     }
    
//     // if (motion_cnt >= 7) {
//     //     alarm();
//     //     clear_display();
//     //     spi1_display1("Motion Detected"); //when motion is detected display on OLED
//     //     motion_cnt = 0; // resets motion counter
//     // }
//     // else {
//     //     GPIOA -> BSRR |= GPIO_BSRR_BR_3;
//     //     no_motion_ct = 0;
//     // }
// }

void update_hist_sensor() {
    int temp = (GPIOA->IDR & GPIO_IDR_0) ? 1 : 0;  // Read PA0
    hist_sensor = (hist_sensor << 1) | temp;  // Shift left and append new bit
    hist_sensor &= 0xFF;  // Keep it 8 bits (optional for safety)
}

void TIM6_DAC_IRQHandler() {
    TIM6 ->  SR &= ~TIM_SR_UIF;
    update_hist_sensor();
    read_motion();
}

void init_tim6(void) {
    RCC->APB1ENR |= RCC_APB1ENR_TIM6EN;  // Enable TIM6 clock
    TIM6->PSC = 4800 - 1;  // Prescaler for 10ms
    TIM6->ARR = 10 - 1;  // Auto-reload value
    TIM6->DIER |= TIM_DIER_UIE;  // Enable update interrupt
    NVIC->ISER[0] = (1 << TIM6_DAC_IRQn);  // Enable TIM6 IRQ in NVIC
    TIM6->CR1 |= TIM_CR1_CEN;  // Start TIM6
}
