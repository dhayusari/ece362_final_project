// MAIN TO CALL FUNCTIONS
#include "stm32f0xx.h"
#include <stdint.h>
#include <stdio.h>
#include "oled.h"
#include "keypad.h"
#include "alarm.h"
#include "sensor.h"
#include "button.h"
#include "led.h"
#include "tft.h"
#include "lcd.h"

void internal_clock();

int main(){
    internal_clock();
    //enabling ports and initialization
    enable_sensor_ports();
    enable_keypad_ports();
    enable_alarm_ports();
    button_init();
    led_init();
    init_tim7();
    init_spi2();
    spi2_init_oled();
    LCD_Setup();

    int system_state = 0;
    int password;
    led_main(system_state);
    // clear_display();
    enable_sensor();
    init_tim6();
    init_tim7();
    init_spi2();
    spi2_init_oled();

    clear_display();
    spi2_display1("Press Left Btn"); 
    spi2_display2("to Enable");
    
    while(1) {
        if(button9_pressed) {
            button9_pressed = 0;
            password = oled_checkpasscode();
            if (password) {
                system_state = 1;
                led_main(system_state);
                system_state = 2;
                countdown();
                clear_display();
                spi2_display1("Detecting Motion"); 

                LCD_DrawLine(0, 28, 200, 28, 0xFFFF);
                LCD_DrawString(15, 30, 0xFFFF, 0xFFFF, "Detecting Motion", 16, 1);
                LCD_DrawLine(0, 42, 200, 42, 0xFFFF);


                LCD_Clear(0xFFFF);
                LCD_DrawString(85, 215, 0xFFFF, 0x0000, "DETECTING MOTION", 16, 1);

                if (motion_detected == 1) {
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
                else if (motion_detected == 0) {
                    LCD_Clear(0xFFFF);
                    LCD_DrawString(85, 215, 0xFFFF, 0x0000, "NO MOTION", 16, 1);
                }
                //clear_display();
                //spi2_display1("Detecting Motion"); 
            } 
            else {
                system_state = 3;
                // led_main(system_state);
                alarm();
            }
        }
        if(button10_pressed && system_state) { //Check to make sure the system is on in order for this button to work
            button10_pressed = 0;
            password = oled_checkpasscode();
            if (password) {
                system_state = 0;
                led_main(system_state);
                disable_sensor();
                disable_alarm();
                clear_display();
                spi2_display1("Disabled Sensor");

                //LCD_Clear(0x77DD);
                LCD_DrawLine(0, 58, 200, 58, 0x77DD);
                LCD_DrawString(15, 60, 0xFFFF, 0xFFFF, "Disabled Sensor!", 16, 1);
                LCD_DrawLine(0, 72, 200, 72, 0x77DD);
            } 
        }
    }
    
}

