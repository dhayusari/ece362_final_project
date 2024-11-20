// MAIN TO CALL FUNCTIONS
#include "stm32f0xx.h"
#include <stdint.h>
#include "oled.h"
#include "keypad.h"
#include "alarm.h"
#include "sensor.h"
#include "button.h"
#include "motion_sensor.h"

void internal_clock();

int main(){
    internal_clock();
    //enabling ports
    enable_sensor_ports();
    enable_keypad_ports();
    enable_alarm_ports();
    enable_ports_oled();
    //initialize button
    button();
    
    // keypad();
    while(1) {
        if(button9_pressed == 1) {
            button9_pressed = 0;
            int password = oled_checkpasscode;
            if (password) {
                enable_sensor();
                init_tim6();
                clear_display();
                spi1_display1("Detecting Motion");
                char key = get_keypress();
                if (key == 'A') {
                   password = oled_checkpasscode();
                if (password) {
                    disable_sensor();
                    clear_display();
                    spi1_display1("Disabled Sensor");
                } else {
                    alarm();
                }
                }
            } else {
                alarm();
            }
        }
        if(button10_pressed == 1) {
            button10_pressed = 0;
            disable_sensor();
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