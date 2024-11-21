#ifndef __LED_H__
#define __LED_H__

#include <stdint.h>

// Function prototypes for LED control
void led_init(void);
void set_color(uint32_t red, uint32_t green, uint32_t blue);
void led_off(void);
void white_pulse(int system_state);
void red_flash(void);
void green(void);
void green_flash(void);
void led_main(int system_state);

#endif /* __LED_H__ */
