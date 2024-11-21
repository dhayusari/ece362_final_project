#ifndef BUTTON_H
#define BUTTON_H

#include <stdint.h>

extern volatile uint32_t button9_pressed;
extern volatile uint32_t button10_pressed;

void button(void);

#endif // BUTTON_H