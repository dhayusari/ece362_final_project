// MOTION SENSOR INITIALIZATION AND FUNCTIONS
#include "stm32f0xx.h"
#include <stdint.h>
#include "keypad.h"
#include "oled.h"
#include "sensor.h"
#include "alarm.h"
#include "lcd.h"
#include "tft.h"

void internal_clock();

uint8_t hist_sensor; // 8 sample bits of input
int motion_cnt = 0;
int no_motion_ct = 0;
int motion = 0;
int state[2] = {0, 0};
int condition = 100;

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
    if (motion > 0) {
        condition = 100;
    }
    if (hist_sensor & 0xFF) {  // Check if the latest bit indicates motion
        motion_cnt++;
        no_motion_ct = 0;  // Reset no motion counter
        if (motion_cnt >= condition) {  // Threshold for alarm
            motion_cnt = 0;  // Reset motion counter
            state[0] = state[1];
            state[1] = 1;
        } 
    } else {  // No motion detected
        motion_cnt = 0;
        no_motion_ct++;
        if (no_motion_ct >= 8) {  // Reset after consecutive no-motion states
            motion_cnt = 0;
            state[0] = state[1];
            state[1] = 0;
        }
        //return 0;
    }
    
}

void update_display() {
    if (state[0] == 0) {
        // clear_display();
        // spi2_display1("No Motion!");

        LCD_Clear(0xFFFF);
        LCD_DrawString(85, 215, 0xFFFF, 0x0000, "NO", 16, 1);
        LCD_DrawString(85, 215, 0xFFFF, 0x0000, "MOTION", 16, 1);
    }
    if (state[0] != state[1]) {
        if (state[1] == 1) {
            if (motion > 8) {
                TIM6 ->CR1 &= ~(TIM_CR1_CEN);
                clear_display();
                spi2_display1("Motion Detected!");

                LCD_DrawLine(0, 42, 200, 42, 0xFF00);
                LCD_DrawString(15, 45, 0xFFFF, 0xFFFF, "Motion Detected!!", 16, 1);
                LCD_DrawLine(0, 67, 200, 67, 0xFF00);

                nano_wait(2000000000);
                //countdown();
                alarm();
            }

            LCD_Clear(0xFFFF);
            LCD_DrawString(85, 215, 0xFFFF, 0x0000, "MOTION", 16, 1);
            LCD_DrawString(85, 215, 0xFFFF, 0x0000, "DETECTED", 16, 1);        
        }
    }
}

void update_hist_sensor() {
    int temp = (GPIOA->IDR & GPIO_IDR_0) ? 1 : 0;  // Read PA0
    hist_sensor = (hist_sensor << 1) | temp;  // Shift left and append new bit
    hist_sensor &= 0xFF;  // Keep it 8 bits (optional for safety)
}

void TIM6_DAC_IRQHandler() {
    TIM6 ->  SR &= ~TIM_SR_UIF;
    update_hist_sensor();
    read_motion();
    update_display();
}

void init_tim6(void) {
    RCC->APB1ENR |= RCC_APB1ENR_TIM6EN;  // Enable TIM6 clock
    TIM6->PSC = 4800 - 1;  // Prescaler for 10ms
    TIM6->ARR = 100- 1;  // Auto-reload value
    TIM6->DIER |= TIM_DIER_UIE;  // Enable update interrupt
    NVIC->ISER[0] = (1 << TIM6_DAC_IRQn);  // Enable TIM6 IRQ in NVIC
    TIM6->CR1 |= TIM_CR1_CEN;  // Start TIM6
}
