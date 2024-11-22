// OLED INITIALIZATION AND FUNCTIONS

//things to add
//push button- display start message on oled (play game to get passcode)
//display passcode/enable alarm (all through tft)
//LED corresponding w different steps

#include "stm32f0xx.h"
#include "oled.h"
#include "keypad.h"
#include "led.h"
#include "alarm.h"
#include <string.h>
#include "lcd.h"
#include "tft.h"

uint8_t col; // the column being scanned
uint8_t hist[16];
int msg_index = 0;
uint16_t msg[8] = { 0x0000,0x0100,0x0200,0x0300,0x0400,0x0500,0x0600,0x0700 };
extern const char font[];

void init_tim7(void) {
    RCC->APB1ENR |= RCC_APB1ENR_TIM7EN;
    TIM7->PSC = 479;
    TIM7->ARR = 99;
    TIM7->DIER |= TIM_DIER_UIE;
    NVIC->ISER[0] = 1 << TIM7_IRQn;
    TIM7->CR1 |= TIM_CR1_CEN; 

}

void TIM7_IRQHandler(){
    TIM7->SR &= ~TIM_SR_UIF;

    int rows = read_rows();
    update_history(col, rows);
    col = (col + 1) & 3;
    drive_column(col);
}

void init_spi2() {
    //enable clock
    RCC->AHBENR |= RCC_AHBENR_GPIOBEN;
    RCC->APB1ENR |= RCC_APB1ENR_SPI2EN;
    
    GPIOB -> MODER &= ~(GPIO_MODER_MODER12 | GPIO_MODER_MODER13 | GPIO_MODER_MODER15);
    GPIOB -> MODER |= GPIO_MODER_MODER15_1 | GPIO_MODER_MODER13_1 | GPIO_MODER_MODER12_1;
    GPIOB->AFR[1] &= ~(GPIO_AFRH_AFRH7 | GPIO_AFRH_AFRH5 | GPIO_AFRH_AFRH4);
    

    SPI2 -> CR1 &= ~(SPI_CR1_SPE);
    SPI2 -> CR1 |= SPI_CR1_BR;
    SPI2 -> CR2 = SPI_CR2_DS_3 | SPI_CR2_DS_0;
    SPI2 -> CR1 |= SPI_CR1_MSTR;
    SPI2 -> CR2 |= SPI_CR2_SSOE | SPI_CR2_NSSP;
    SPI2 -> CR2 |= SPI_CR2_TXDMAEN;
    SPI2 -> CR1 |= SPI_CR1_SPE;
}

void spi_cmd(unsigned int data) {
    while((SPI2->SR & SPI_SR_TXE) == 0);
    SPI2->DR = data;  
}

void spi_data(unsigned int data) {
    spi_cmd(data |0x200);   
}

void spi2_init_oled() {
    nano_wait(1000000);
    spi_cmd(0x38);
    spi_cmd(0x08);
    spi_cmd(0x01);
    nano_wait(2000000);
    spi_cmd(0x06);
    spi_cmd(0x02);
    spi_cmd(0x0c);
}

void spi2_display1(const char *string) {
  spi_cmd(0x02);
  while (*string != '\0') {
    spi_data(*string);
    string++;
  }
}

void spi2_display2(const char *string) {
    spi_cmd(0xc0);
    while (*string != '\0') {
        spi_data(*string);
        string++;
    }
}

# define MAX_DIGITS 4
//predefined passcode
const char predefined_passcode[] = "1234";
char entered_digits[MAX_DIGITS + 1] = {0};
int digit_index = 0;

void append_digit(char digit) {
    if (digit_index < MAX_DIGITS) {
        entered_digits[digit_index++] = digit;
        entered_digits[digit_index] = '\0';
    }
}

int check_passcode() {
    if (strcmp(entered_digits, predefined_passcode) == 0){
        clear_display();
        spi2_display2("MATCHED!");
        reset_passcode_entry();
        return 1;
    }
    else{
        clear_display();
        spi2_display2("INCORRECT");
        return 0;
    }
}

void nano_wait(unsigned int n) {
    asm(    "        mov r0,%0\n"
            "repeat: sub r0,#83\n"
            "        bgt repeat\n" : : "r"(n) : "r0", "cc");
}

void reset_passcode_entry(void) {
    //clear entered_digits for new attempt
    digit_index = 0;
    entered_digits[0] = '\0';
}

void clear_display(void) {
    spi_cmd(0x01); // Command to clear display
    nano_wait(2000000); // Wait for the clear command to complete
}

int oled_checkpasscode(void) {
    clear_display();
    spi2_display1("Enter passcode:");
    
    LCD_Clear(0x0000);
    LCD_DrawLine(0, 13, 200, 13, 0xFFFF);
    LCD_DrawString(15, 15, 0xFFFF, 0xFFFF, "Enter", 16, 1);
    LCD_DrawString(65, 15, 0xFFFF, 0xFFFF, "Passcode:", 16, 1);
    LCD_DrawLine(0, 35, 200, 35, 0xFFFF);


    int attempts = 0; //counter
    #define MAX_ATTEMPTS 3 //after max reached exit to alarm

    while(attempts < MAX_ATTEMPTS){
        char key = get_keypress();

        if (key >= '0' && key <= '9'){
            append_digit(key);
            spi2_display2(entered_digits);
        }
        else if (key == '#'){
            if (check_passcode()){
                //led_main(1);
                return 1;
            }
            else{
                // clear_display();
                // spi2_display1("Re-Enter Code");
                LCD_Clear(0xFFFF);
                LCD_DrawString(85, 215, 0xFFFF, 0x0000, "Re-Enter", 16, 1);
                LCD_DrawString(85, 215, 0xFFFF, 0x0000, "Code:", 16, 1);
                attempts++;
            }
            digit_index = 0;
            entered_digits[0] = '\0';
            nano_wait(3000000000);
        }
    }
    if (attempts == MAX_ATTEMPTS){
       alarm();
       return 0;
    }
    return 0;
}