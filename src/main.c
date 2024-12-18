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
                green();
                system_state = 2;
                countdown();
                clear_display();
                enable_sensor();
                //spi2_display1("Detecting Motion");

                LCD_DrawString(15, 35, 0xFFFF, 0xFFFF, "Detecting Motion", 16, 1);

                init_tim6();

                //clear_display();
                //spi2_display1("Detecting Motion"); 
            } 
            else {
                system_state = 3;
                led_main(system_state);
                alarm();
            }
        }
        else if(button10_pressed && system_state) { //Check to make sure the system is on in order for this button to work
            button10_pressed = 0;
            password = oled_checkpasscode();
            if (password) {
                system_state = 0;
                led_main(system_state);
                disable_sensor();
                disable_alarm();
                clear_display();
                //spi2_display1("Disabled Sensor");

                //LCD_Clear(0x77DD);
                LCD_DrawString(15, 60, 0xFFFF, 0xFFFF, "Disabled Sensor", 16, 1);
            } 
        }
    }
    
}

