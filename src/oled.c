// OLED INITIALIZATION AND FUNCTIONS

//things to add
//push button- display start message on oled (play game to get passcode)
//display passcode/enable alarm (all through tft)
//LED corresponding w different steps

#include "stm32f0xx.h"
#include "oled.h"
#include "keypad.h"
#include <string.h>

uint8_t col; // the column being scanned
uint8_t hist[16];
int msg_index = 0;
uint16_t msg[8] = { 0x0000,0x0100,0x0200,0x0300,0x0400,0x0500,0x0600,0x0700 };
extern const char font[];

void internal_clock();
void alarm(void);

void oled_main_startingmsg(void);

void enable_ports_oled(void) {
    // Only enable port C for the keypad
    RCC->AHBENR |= RCC_AHBENR_GPIOCEN;
    GPIOC->MODER &= ~0xffff;
    GPIOC->MODER |= 0x55 << (4*2);
    GPIOC->OTYPER &= ~0xff;
    GPIOC->OTYPER |= 0xf0;
    GPIOC->PUPDR &= ~0xff;
    GPIOC->PUPDR |= 0x55;
}

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

void init_spi1() {
    //enable clock
    RCC->AHBENR |= RCC_AHBENR_GPIOAEN;
    RCC->APB2ENR |= RCC_APB2ENR_SPI1EN;
    
    GPIOA -> MODER &= ~(0xC000CC00);
    GPIOA -> MODER |= 0x80008800;
    GPIOA->AFR[1] &= ~(GPIO_AFRH_AFRH7);
    GPIOA->AFR[0] &= ~(GPIO_AFRL_AFRL5 | GPIO_AFRL_AFRL7);

    SPI1 -> CR1 &= ~(SPI_CR1_SPE);
    SPI1 -> CR1 |= SPI_CR1_BR;
    SPI1 -> CR2 = SPI_CR2_DS_3 | SPI_CR2_DS_0;
    SPI1 -> CR1 |= SPI_CR1_MSTR;
    SPI1 -> CR2 |= SPI_CR2_SSOE | SPI_CR2_NSSP;
    SPI1 -> CR2 |= SPI_CR2_TXDMAEN;
    SPI1 -> CR1 |= SPI_CR1_SPE;
}

void spi_cmd(unsigned int data) {
    while((SPI1->SR & SPI_SR_TXE) == 0);
    SPI1->DR = data;  
}

void spi_data(unsigned int data) {
    spi_cmd(data |0x200);   
}

void spi1_init_oled() {
    nano_wait(1000000);
    spi_cmd(0x38);
    spi_cmd(0x08);
    spi_cmd(0x01);
    nano_wait(2000000);
    spi_cmd(0x06);
    spi_cmd(0x02);
    spi_cmd(0x0c);
}

void spi1_display1(const char *string) {
  spi_cmd(0x02);
  while (*string != '\0') {
    spi_data(*string);
    string++;
  }
}

void spi1_display2(const char *string) {
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
        spi1_display2("MATCHED!");
        reset_passcode_entry();
        return 1;
    }
    else{
        spi1_display2("INCORRECT");
        return 0;
    }
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

void oled_main_startingmsg(void){
    clear_display();
    spi1_display1("Welcome");
    nano_wait(2000000);
    clear_display();
    spi1_display1("Play Game To");
    spi1_display2("Get Passcode");
}

int oled_checkpasscode(void) {
    internal_clock();
    init_tim7();
    init_spi1();
    spi1_init_oled();
    clear_display();
    spi1_display1("Enter passcode:");

    int attempts = 0; //counter
    #define MAX_ATTEMPTS 3 //after max reached exit to alarm

    while(attempts < MAX_ATTEMPTS){
        char key = get_keypress();

        if (key >= '0' && key <= '9'){
            append_digit(key);
            spi1_display2(entered_digits);
        }
        else if (key == '#'){
            if (check_passcode()){
                return 1;
            }
            else{
                clear_display();
                spi1_display1("Re-Enter Code");
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