/*
 * HC-SR04.c
 *
 *  Created on: 26.03.2019
 *      Author: Admin
 *
 *	TRIG		- PB8, TIM4_CH3
 *	ECHO		- PB9, TIM4_CH4
 */
#include "HCSR04.h"

// -----------------------------------------------
// private variables
// -----------------------------------------------
__IO uint32_t t0 = 0, delta_us = 0;
__IO uint16_t distance = 0;
__IO uint8_t echo_flag = 0;
__IO uint8_t tim2_it_count = 0;
// -----------------------------------------------

// ------------------------------------------------------------
// Variant 1 (Init GPIO)
// ------------------------------------------------------------
void HCSR04_Init() {

	RCC->APB2ENR |= RCC_APB2ENR_IOPBEN;	// IOPBEN: IO port B clock enable

	// TRIG pin Output
	GPIOB->CRH &= ~GPIO_CRH_MODE8;		// Reset
	GPIOB->CRH |= GPIO_CRH_MODE8_0;		// 01: Output mode, max speed 10 MHz.
	GPIOB->CRH &= ~GPIO_CRH_CNF8;		// 00: General purpose output push-pull
	GPIOB->ODR &= ~GPIO_ODR_ODR8;		// pull-down

	// ECHO pin Input
	GPIOB->CRH &= ~GPIO_CRH_MODE9;		// 00: Input mode (reset state)
	GPIOB->CRH &= ~GPIO_CRH_CNF9;		// Reset
	GPIOB->CRH |= GPIO_CRH_CNF9_1;		// 10: Input with pull-up / pull-down
	GPIOB->ODR &= ~GPIO_ODR_ODR9;		// pull-down
}

// ------------------------------------------------------------
// Variant 1 (TRIG and Read with loops)
// ------------------------------------------------------------
uint32_t HCSR04_Read() {

	__disable_irq();

	uint32_t _start = 0, _estimate_us = 0, _distance = 0;

	// Set TRIG to high level
	GPIOB->ODR |= GPIO_ODR_ODR8;
	// Wait 10us
	DWT_delay_us(10);
	// Set TRIG to low level
	GPIOB->ODR &= ~GPIO_ODR_ODR8;

	// Wait until ECHO is LOW
	while ((GPIOB->IDR & GPIO_IDR_IDR9) == 0)
		;
	// Get clock
	_start = DWT_tick();
	// Wait until ECHO is HIGH
	while ((GPIOB->IDR & GPIO_IDR_IDR9) != 0)
		;
	// Estimate time in us
	_estimate_us = (DWT_tick() - _start) / (SystemCoreClock / 1000000);
	// Calculate distance in cm
	// For accuracy 0.3mm
	// S_cm = 17 * T_us / 10**3
	// S_hcm = sqrt(S_cm**2 - 2.25)
	_distance = _estimate_us / 58;

	__enable_irq();

	return _distance;
}

// ------------------------------------------------------------
// Variant 2 (Init with external interrupt)
// ------------------------------------------------------------
void HCSR04_InitIT() {

	RCC->APB2ENR |= RCC_APB2ENR_IOPBEN;			// IOPBEN: IO port B clock enable
	RCC->APB2ENR |= RCC_APB2ENR_AFIOEN;			// AFIOEN: Alternate function IO clock enable

	// TRIG pin Output
	GPIOB->CRH &= ~GPIO_CRH_MODE8;				// Reset
	GPIOB->CRH |= GPIO_CRH_MODE8_0;				// 01: Output mode, max speed 10 MHz.
	GPIOB->CRH &= ~GPIO_CRH_CNF8;				// 00: General purpose output push-pull
	GPIOB->ODR &= ~GPIO_ODR_ODR8;				// pull-down

	// ECHO pin Input
	GPIOB->CRH &= ~GPIO_CRH_MODE9;				// 00: Input mode (reset state)
	GPIOB->CRH &= ~GPIO_CRH_CNF9;				// Reset
	GPIOB->CRH |= GPIO_CRH_CNF9_1;				// 10: Input with pull-up / pull-down
	GPIOB->ODR &= ~GPIO_ODR_ODR9;				// pull-down

	// External interrupt on ECHO pin
	AFIO->EXTICR[2] = 0;						// Reset
	AFIO->EXTICR[2] |= AFIO_EXTICR3_EXTI9_PB;	// EXTIx[3:0]: EXTI x configuration (x= 8 to 11), 0001: PB[x] pin
	EXTI->IMR |= EXTI_IMR_MR9;					// MRx: Interrupt Mask on line x, 1: Interrupt request from Line x is not masked
	EXTI->RTSR |= EXTI_RTSR_TR9;				// 1: Rising trigger enabled (for Event and Interrupt) for input line.
	EXTI->FTSR |= EXTI_FTSR_TR9;				// 1: Falling trigger enabled (for Event and Interrupt) for input line.
	NVIC_EnableIRQ(EXTI9_5_IRQn);				// Enable External Interrupt, External Line[9:5] Interrupts
	NVIC_SetPriority(EXTI9_5_IRQn, 0);			// Set Interrupt Priority

	// Timers
	RCC->APB1ENR |= RCC_APB1ENR_TIM2EN;				// TIM2EN: TIM2 timer clock enable
	TIM2->PSC = (SystemCoreClock / 1000000 - 1);	// PSC[15:0]: Prescaler value, 1uS tick.
	TIM2->ARR = 38000 - 1;							// ARR[15:0]: Prescaler value, 38mS
	TIM2->DIER |= TIM_DIER_UIE;						// 1: Update interrupt enabled.
	NVIC_EnableIRQ(TIM2_IRQn);						// Enable External Interrupt, TIM2 global Interrupt
}

// ------------------------------------------------------------
// Variant 2 (TRIG with GPIO and delay)
// ------------------------------------------------------------
uint32_t HCSR04_ReadIT() {

	if (echo_flag == 0) {
		__disable_irq();

		// Set TRIG to high level
		GPIOB->ODR |= GPIO_ODR_ODR8;
		// Wait 10us
		DWT_delay_us(10);
		// Set TRIG to low level
		GPIOB->ODR &= ~GPIO_ODR_ODR8;

		__enable_irq();

		return distance;
	}

	return 0;
}

// ------------------------------------------------------------
// Variant 3 (Init and TRIG with PWM and external interrupt)
// ------------------------------------------------------------
void HCSR04_InitPWM() {
	RCC->APB2ENR |= RCC_APB2ENR_IOPBEN;			// IOPBEN: IO port B clock enable
	RCC->APB2ENR |= RCC_APB2ENR_AFIOEN;			// AFIOEN: Alternate function IO clock enable

	// TRIG pin Output
	GPIOA->CRH |= GPIO_CRH_MODE8;				// 11: Output mode, max speed 50 MHz.
	GPIOA->CRH &= ~GPIO_CRH_CNF8;				// Reset
	GPIOA->CRH |= GPIO_CRH_CNF8_1;				// 10: Alternate function output Push-pull

	// ECHO pin Input
	GPIOB->CRH &= ~GPIO_CRH_MODE9;				// 00: Input mode (reset state)
	GPIOB->CRH &= ~GPIO_CRH_CNF9;				// Reset
	GPIOB->CRH |= GPIO_CRH_CNF9_1;				// 10: Input with pull-up / pull-down
	GPIOB->ODR &= ~GPIO_ODR_ODR9;				// pull-down

	// External interrupt on ECHO pin
	AFIO->EXTICR[2] = 0;						// Reset
	AFIO->EXTICR[2] |= AFIO_EXTICR3_EXTI9_PB;	// EXTIx[3:0]: EXTI x configuration (x= 8 to 11), 0001: PB[x] pin
	EXTI->IMR |= EXTI_IMR_MR9;					// MRx: Interrupt Mask on line x, 1: Interrupt request from Line x is not masked
	EXTI->RTSR |= EXTI_RTSR_TR9;				// 1: Rising trigger enabled (for Event and Interrupt) for input line.
	EXTI->FTSR |= EXTI_FTSR_TR9;				// 1: Falling trigger enabled (for Event and Interrupt) for input line.
	NVIC_EnableIRQ(EXTI9_5_IRQn);				// Enable External Interrupt, External Line[9:5] Interrupts
	NVIC_SetPriority(EXTI9_5_IRQn, 0);			// Set Interrupt Priority

	// PWM on TRIG pin
	RCC->APB1ENR |= RCC_APB1ENR_TIM4EN;					// TIM4EN: Timer 4 clock enable
	TIM4->PSC = (SystemCoreClock / 1000000 - 1);		// PSC[15:0]: Prescaler value, 1uS tick.
	TIM4->ARR = 60000 - 1;								// ARR[15:0]: Prescaler value, 60mS
	TIM4->CCR3 = 10;									// CCR3[15:0]: Capture/Compare value, 10uS
	TIM4->CCMR2 |= TIM_CCMR2_OC3M_2 | TIM_CCMR2_OC3M_1;	// 110: PWM mode 1 - In upcounting, channel 3 is active as long as TIMx_CNT<TIMx_CCR3 else inactive.
	TIM4->CCER |= TIM_CCER_CC3E;						// CC3E: Capture/Compare 3 output enable
	TIM4->CR1 |= TIM_CR1_CEN;							// 1: Counter enabled

	// Timers
	RCC->APB1ENR |= RCC_APB1ENR_TIM2EN;				// TIM2EN: TIM2 timer clock enable
	TIM2->PSC = (SystemCoreClock / 1000000 - 1);	// PSC[15:0]: Prescaler value, 1uS tick.
	TIM2->ARR = 38000 - 1;							// ARR[15:0]: Prescaler value, 38mS
	TIM2->DIER |= TIM_DIER_UIE;						// 1: Update interrupt enabled.
	NVIC_EnableIRQ(TIM2_IRQn);						// Enable External Interrupt, TIM2 global Interrupt
}

// ------------------------------------------------------------
// Variant 3 (Read with PWM)
// ------------------------------------------------------------
uint32_t HCSR04_ReadPWM() {
	return distance;
}

void EXTI9_5_IRQHandler() {

//	// DWT variant
//	if (EXTI->PR & EXTI_PR_PR9) {
//		EXTI->PR |= EXTI_PR_PR9;	// 1: selected trigger request occurred. This bit is cleared by writing a '1' into the bit.
//
//		if ((GPIOB->IDR & GPIO_IDR_IDR9)) {
//			t0 = DWT_tick();
//			echo_flag = 1;
//		} else {
//			if (echo_flag) {
//				delta_us = (DWT_tick() - t0) / (SystemCoreClock / 1000000);
//				distance = delta_us / 58;
//				echo_flag = 0;
//			}
//		}
//	}

	// TIM2 variant
	if (EXTI->PR & EXTI_PR_PR9) {
		EXTI->PR |= EXTI_PR_PR9;	// 1: selected trigger request occurred. This bit is cleared by writing a '1' into the bit.

		if ((GPIOB->IDR & GPIO_IDR_IDR9)) {
			TIM2->CR1 |= TIM_CR1_CEN;			// 1: Counter enabled
			echo_flag = 1;
		} else {
			if (echo_flag) {
				TIM2->CR1 &= ~TIM_CR1_CEN;		// 0: Counter disabled
				distance = TIM2->CNT / 58;
				TIM2->CNT = 0;
				echo_flag = 0;
				tim2_it_count = 0;
			}
		}
	}
}

void TIM2_IRQHandler()
{
	TIM2->SR &= ~TIM_SR_UIF;					// 0: No update occurred.
	tim2_it_count++;
}
