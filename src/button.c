// PUSH BUTTON INITIALIZATION AND FUNCTIONS
#include "stm32f0xx.h"
#include <stdint.h>
#include "button.h"

volatile uint32_t button9_pressed = 0; // Variable to store the value when button 9 is pressed
volatile uint32_t button10_pressed = 0; // Variable to store the value when button 10 is pressed

void button_init(void);
void EXTI4_15_IRQHandler(void);
 
void button_init(void) {
  //Using PC 9-10 as inputs for push buttons
  RCC -> AHBENR |= RCC_AHBENR_GPIOCEN;
  RCC -> APB2ENR |= RCC_APB2ENR_SYSCFGCOMPEN;

  GPIOC->MODER &= ~(GPIO_MODER_MODER9 | GPIO_MODER_MODER10); // Set PC9 and PC10 as input
  GPIOC -> PUPDR |= (GPIO_PUPDR_PUPDR9_1 | GPIO_PUPDR_PUPDR10_1);

  SYSCFG -> EXTICR[2] |= 0x0020;
  SYSCFG -> EXTICR[2] |= 0x0200;
  EXTI -> RTSR |= (EXTI_RTSR_RT9 | EXTI_RTSR_RT10); //Using rising trigger for interrupt
  EXTI -> IMR |= (EXTI_IMR_IM9 | EXTI_IMR_IM10);
  NVIC_EnableIRQ(EXTI4_15_IRQn); 
}

/*Checks pending register for interrupt, and then also checks if interrupt is for 
PC 9 or 10; if true, then saves 1 to respective button variables*/
void EXTI4_15_IRQHandler(void) {
  if(EXTI -> PR & EXTI_PR_PR9) {
    EXTI -> PR |= EXTI_PR_PR9;
    button9_pressed = 1;
  }

  if(EXTI -> PR & EXTI_PR_PR10) {
    EXTI -> PR |= EXTI_PR_PR10;
    button10_pressed = 1;
    TIM6 -> CR1 &= ~(TIM_CR1_CEN);
  }
}