// MAIN TO CALL FUNCTIONS
#include "stm32f0xx.h"
#include <stdint.h>
#include "oled.h"
#include "keypad.h"
#include "alarm.h"
#include "sensor.h"
#include "button.h"
#include "sensor.h"
#include "led.h"

void internal_clock();

int main(){
    internal_clock();
    //enabling ports
    enable_sensor_ports();
    enable_keypad_ports();
    enable_alarm_ports();
    enable_ports_oled();
    //setup_tim1();
    //led_main(0);
    //initialize button
    // button();
    
    // if (button9_pressed) {
    //     GPIOA -> BSRR |= GPIO_BSRR_BS_3;
        
    // }
    // if (button10_pressed) {
    //     GPIOA -> BSRR |= GPIO_BSRR_BR_3;
    // }
    int password = oled_checkpasscode();
    if (password) {
        enable_sensor();
        init_tim6();
        clear_display();
        spi1_display1("Detecting Motion");
        //led_main(2);
        char key = get_keypress();
        if (key == 'A') {
            password = oled_checkpasscode();
        if (password) {
            disable_sensor();
            disable_alarm();
            clear_display();
            spi1_display1("Disabled Sensor");
        } else {
            alarm();
        }
    }
    } else {
        alarm();
    }
        //     button9_pressed = 0;
        // }
        // if(button10_pressed == 1) {
        //     disable_sensor();
        //     button10_pressed = 0;
        // }
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