/*
 * pwm.c
 *
 *  Created on: 20 апр. 2019 г.
 *      Author: Admin
 *
 * PWM - PA0 TIM2_CH1
 */
#include "pwm.h"

void PWM_Init() {

	RCC->APB2ENR |= RCC_APB2ENR_IOPAEN;			// I/O port A clock enable
	RCC->APB2ENR |= RCC_APB2ENR_AFIOEN;			// Alternate Function I/O clock enable
	RCC->APB1ENR |= RCC_APB1ENR_TIM2EN;			// Timer 2 clock enabled

	GPIOA->CRL |= GPIO_CRL_MODE0;				// 11: Output mode, max speed 50 MHz.
	GPIOA->CRL &= ~GPIO_CRL_CNF0;				// Reset
	GPIOA->CRL |= GPIO_CRL_CNF0_1;				// 10: Alternate function output Push-pull

	// PWM
	TIM2->PSC = SystemCoreClock / 1000000 - 1;				// PSC[15:0]: Prescaler value
	TIM2->ARR = 1000 - 1;									// ARR[15:0]: Prescaler value
	TIM2->CCR1 = 0;											// CCR4[15:0]: Capture/Compare value
	TIM2->CCMR1 &= ~TIM_CCMR1_OC1M;							// Reset
	TIM2->CCMR1 |= (TIM_CCMR1_OC1M_2 | TIM_CCMR1_OC1M_1);	// 110: PWM mode 1 - In upcounting, channel 1 is active as long as TIMx_CNT<TIMx_CCR1 else inactive.
	TIM2->CCER |= TIM_CCER_CC1E;							// 1: On - OC1 signal is output on the corresponding output pin.
	TIM2->CR1 |= TIM_CR1_CEN;								// 1: Counter enabled
}

void PWM_Set(uint32_t value) {
	TIM2->CCR1 = value;
}
