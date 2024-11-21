#ifndef __SENSOR_H__
#define __SENSOR_H__

void enable_sensor_ports();
void enable_sensor();
void init_tim6();
void read_motion();
void disable_sensor();
void TIM6_DAC_IRQHandler();
void update_hist_sensor();

#endif /* __SENSOR_H__ */