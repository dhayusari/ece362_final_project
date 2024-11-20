#ifndef __MOTION_SENSOR_H__
#define __MOTION_SENSOR_H__

void enable_sensor_ports();
void enable_sensor();
void disable_sensor();
void read_motion();
void update_hist_sensor();
void init_tim6();
void TIM6_DAC_IRQHandler();

//global variables

#endif /* __KEYPAD_H__ */