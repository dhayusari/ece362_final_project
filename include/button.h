#ifndef __BUTTON_H__
#define __BUTTON_H__

#include <stdint.h>

extern volatile uint32_t button9_pressed;
extern volatile uint32_t button10_pressed;

void button_init(void);
void EXTI4_15_IRQHandler(void);

#endif // BUTTON_H