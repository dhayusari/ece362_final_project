#ifndef __OLED_H__
#define __OLED_H__

void init_tim7();
void TIM7_IRQHandler();
void init_spi2();
void spi_cmd(unsigned int data);
void spi_data(unsigned int data);
void nano_wait(unsigned int n);
void spi2_init_oled();
void spi2_display1(const char *string);
void spi2_display2(const char *string);
void append_digit(char digit);
int check_passcode();
void reset_passcode_entry();
void clear_display();
int oled_checkpasscode();

#endif /* __OLED_H__ */