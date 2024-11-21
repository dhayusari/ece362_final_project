#ifndef __TFT_H__
#define __TFT_H__

void init_spi1_slow(void);
void enable_sdcard(void);
void disable_sdcard(void);
void init_sdcard_io(void);
void sdcard_io_high_speed(void);
void init_lcd_spi(void);

#endif /* __TFT_H__ */