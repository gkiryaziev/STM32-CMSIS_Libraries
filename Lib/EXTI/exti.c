/*
 * exti.c
 *
 *  Created on: 31.03.2019
 *      Author: Admin
 */
#include "exti.h"

void EXTI_Init(){

	RCC->APB2ENR |= RCC_APB2ENR_IOPBEN;			// IOPBEN: IO port B clock enable
	RCC->APB2ENR |= RCC_APB2ENR_AFIOEN;			// AFIOEN: Alternate function IO clock enable

	// ECHO pin Input
	GPIOB->CRH &= ~GPIO_CRH_MODE9;				// 00: Input mode (reset state)
	GPIOB->CRH &= ~GPIO_CRH_CNF9;				// Reset
	GPIOB->CRH |= GPIO_CRH_CNF9_1;				// 10: Input with pull-up / pull-down
	GPIOB->ODR &= ~GPIO_ODR_ODR9;

	// External interrupt on ECHO pin
	AFIO->EXTICR[2] = 0;						// Reset
	AFIO->EXTICR[2] |= AFIO_EXTICR3_EXTI9_PB;	// EXTIx[3:0]: EXTI x configuration (x= 8 to 11), 0001: PB[x] pin
	EXTI->IMR |= EXTI_IMR_MR9;					// MRx: Interrupt Mask on line x, 1: Interrupt request from Line x is not masked
	EXTI->RTSR |= EXTI_RTSR_TR9;				// 1: Rising trigger enabled (for Event and Interrupt) for input line.
	EXTI->FTSR |= EXTI_FTSR_TR9;				// 1: Falling trigger enabled (for Event and Interrupt) for input line.
	NVIC_EnableIRQ(EXTI9_5_IRQn);				// Enable External Interrupt, External Line[9:5] Interrupts
}

void EXTI9_5_IRQHandler() {

	EXTI->PR |= EXTI_PR_PR9;					// 1: selected trigger request occurred. This bit is cleared by writing a ‘1’ into the bit.

	if ((GPIOB->IDR & GPIO_IDR_IDR9) != 0) {
		GPIOC->BSRR |= GPIO_BSRR_BS13;
	} else {
		GPIOC->BSRR |= GPIO_BSRR_BR13;
	}
}
