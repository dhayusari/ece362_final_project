#ifndef LED_H
#define LED_H

#include <stdint.h>

// Function prototypes for LED control
void led_init(void);
void set_color(uint16_t red, uint16_t green, uint16_t blue);
void led_off(void);
void white_pulse(void);
void red_flash(void);
void green(void);
void green_flash(void);
void led_main(int system_state);

#endif // LED_H