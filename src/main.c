// MAIN TO CALL FUNCTIONS

#include "stm32f0xx.h"
#include <stdint.h>

void internal_clock();
// void enable_sensor_ports();
// void enable_keypad_ports();
void oled_main();

int main(){
    internal_clock();
<<<<<<< HEAD
    keypad();

=======

    oled_main();
>>>>>>> 61c682585c26b229753f51ea1ffe513ea8fb6b11
}