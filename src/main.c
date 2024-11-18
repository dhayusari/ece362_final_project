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
int oled_checkpasscode();
void init_tim6();
void alarm();
void clear_display();
void spi1_display1(const char *string);
void read_motion();


int main(){
    internal_clock();
    //enabling ports
    enable_sensor_ports();
    enable_keypad_ports();
    enable_alarm_ports();
    enable_ports_oled();
    // keypad();

    // functionality of the system
    int password = oled_checkpasscode();
    if (password) {
        enable_sensor();
        init_tim6();
        clear_display();
        spi1_display1("Detecting Motion");
    }
    if (password == 0) {
        alarm();
    }

    
}