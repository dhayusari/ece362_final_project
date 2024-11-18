// MAIN TO CALL FUNCTIONS

#include "stm32f0xx.h"
#include <stdint.h>
#include "oled.h"
#include "keypad.h"

void internal_clock();
void enable_sensor_ports();
void enable_ports_oled();
void enable_alarm_ports();
void enable_keypad_ports();
void enable_sensor();
int oled_main();

int main(){
    internal_clock();
    //enabling ports
    enable_sensor_ports();
    enable_keypad_ports();
    enable_alarm_ports();
    enable_ports_oled();
    // keypad();

    // functionality of the system
    int password = oled_main();
    if (password) {
        enable_sensor();
    }

    
}