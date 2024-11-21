// TFT DISPLAY INITIALIZATION AND FUNCTIONS
#include "stm32f0xx.h"

void init_spi1_slow(void) {
    RCC -> AHBENR |= RCC_AHBENR_GPIOBEN; // Enable GPIOB clock
    RCC -> APB2ENR |= RCC_APB2ENR_SPI1EN; // Enable SPI1 clock

    // Configure GPIOB MODER and AFR registers for pins PB3 (SCK), PB4 (MISO), and PB5 (MOSI)
    GPIOB -> MODER &= ~(GPIO_MODER_MODER3 | GPIO_MODER_MODER4 | GPIO_MODER_MODER5); // Clear current mode
    GPIOB -> MODER |= (GPIO_MODER_MODER3_1 | GPIO_MODER_MODER4_1 | GPIO_MODER_MODER5_1); // Set to Alternate Function
    GPIOB -> AFR[0] &= ~((0xF << (3 * 4)) | (0xF << (4 * 4)) | (0xF << (5 * 4))); // Clear AF
    GPIOB -> AFR[0] |= ((0x0 << (3 * 4)) | (0x0 << (4 * 4)) | (0x0 << (5 * 4))); // Set AF0 (SPI1)

    SPI1 -> CR1 &= ~SPI_CR1_SPE; // Disable SPI

    SPI1 -> CR1 |= SPI_CR1_BR; // Set the baud rate divisor to the maximum value to make the SPI baud rate as low as possible.

    SPI1 -> CR1 |= SPI_CR1_MSTR; // Set it to "Master Mode".

    SPI1 -> CR2 |= (SPI_CR2_DS_2 | SPI_CR2_DS_1 | SPI_CR2_DS_0); // Set the word (data) size to 8-bit.

    SPI1 -> CR1 |= SPI_CR1_SSM | SPI_CR1_SSI; // Configure "Software Slave Management" and "Internal Slave Select".

    SPI1 -> CR2 |= SPI_CR2_FRXTH; // Set the "FIFO reception threshold" bit in CR2 so that the SPI channel immediately releases a received 8-bit value.
    
    SPI1 -> CR1 |= SPI_CR1_SPE; // Enable the SPI channel.
}

void enable_sdcard(void) {
    // This function should set PB2 low to enable the SD card.
    GPIOB -> BRR |= GPIO_BRR_BR_2;
}

void disable_sdcard(void) {
    // This function should set PB2 high to disable the SD card.
    GPIOB -> BSRR |= GPIO_BSRR_BS_2;
}

void init_sdcard_io(void) {
    init_spi1_slow(); // Calls init_spi1_slow().

    // Configures PB2 as an output.
    GPIOB->MODER &= ~GPIO_MODER_MODER2;
    GPIOB->MODER |= GPIO_MODER_MODER2_0;

    disable_sdcard(); // Calls disable_sdcard().
}

void sdcard_io_high_speed(void) {
    SPI1 -> CR1 &= ~SPI_CR1_SPE; // Disable the SPI1 channel.

    // Set the SPI1 Baud Rate register so that the clock rate is 12 MHz. (You may need to set this lower if your SD card does not reliably work at this rate.)
    SPI1 -> CR1 &= ~SPI_CR1_BR;
    SPI1 -> CR1 |= SPI_CR1_BR_0;

    SPI1 -> CR1 |= SPI_CR1_SPE; // Re-enable the SPI1 channel.
}

void init_lcd_spi(void) {
    RCC -> AHBENR |= RCC_AHBENR_GPIOBEN;

    // Configure PB8, PB11, and PB14 as GPIO outputs
    GPIOB -> MODER &= ~(GPIO_MODER_MODER8 | GPIO_MODER_MODER11 | GPIO_MODER_MODER14); // Clear current modes
    GPIOB -> MODER |= (GPIO_MODER_MODER8_0 | GPIO_MODER_MODER11_0 | GPIO_MODER_MODER14_0); // Set as outputs

    init_spi1_slow(); // Call init_spi1_slow() to configure SPI1.

    sdcard_io_high_speed(); // Call sdcard_io_high_speed() to make SPI1 fast.
}
//end with displaying password