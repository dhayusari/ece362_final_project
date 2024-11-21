// MAIN TO CALL FUNCTIONS
#include "stm32f0xx.h"
#include <stdint.h>
// #include "oled.h"
// #include "keypad.h"
// #include "alarm.h"
// #include "sensor.h"
#include "button.h"
// #include "motion_sensor.h"
#include "led.h"

// void internal_clock();

int main(){
    // internal_clock();
    // //enabling ports
    // enable_sensor_ports();
    // enable_keypad_ports();
    // enable_alarm_ports();
    // enable_ports_oled();
    // //initialize button
    button_init();
    led_init();
        // keypad();
    while(1) {
        if(button9_pressed) {
            // button9_pressed = 0;
            // int password = oled_checkpasscode;
            // if(password) {
            //     enable_sensor();
            //     init_tim6();
            //     clear_display();
            //     spi1_display1("Detecting Motion");
            //     char key = get_keypress();
            //     if(key == 'A') {
            //        password = oled_checkpasscode();
            //         if (password) {
            //             disable_sensor();
            //             clear_display();
            //             spi1_display1("Disabled Sensor");
            //         } 
            //         else {
            //             alarm();
            //         }
            //     }
            // } 
            // else {
            //     alarm();
            // }
            button9_pressed = 0;
            green_flash();
        }
        else if(button10_pressed) {
            button10_pressed = 0;
            red_flash();
            //disable_sensor();
        }
        else {
            white_pulse();
        }
    }
    // char key = get_keypress();
    // functionality of the system
    //int password = oled_checkpasscode();
    // if (password) {
    //     enable_sensor();
    //     init_tim6();
    //     clear_display();
    //     spi1_display1("Detecting Motion");
    //     char key = get_keypress();
    //     if (key == 'A') {
    //         password = oled_checkpasscode();
    //         if (password) {
    //             disable_sensor();
    //             clear_display();
    //             spi1_display1("Disabled Sensor");
    //         } else {
    //             alarm();
    //         }
    //     }
    // } else {
    //     alarm();
    // }

    
}