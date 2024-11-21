#ifndef __LED_H__
#define __LED_H__

void nano_wait(unsigned int n);
void setup_tim1(void);
void set_color(uint16_t red, uint16_t green, uint16_t blue);
void led_off(void);
void white_pulse(void);
void red_flash(int curr_state);
void green(void);
void green_flash(void);
void led_main(int system_state);

#endif /* __LED_H__ */