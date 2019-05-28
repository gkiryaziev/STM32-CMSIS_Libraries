/*
 * USART1
 * APB CLOCK - 72MHz
 * BAUD RATE - 115200
 * TX        - PA9
 * RX        - PA10
 */
#include "usart.h"

void USART1_Init() {

	RCC->APB2ENR |= RCC_APB2ENR_USART1EN;		// 1: USART1 clock enabled
	RCC->APB2ENR |= RCC_APB2ENR_IOPAEN;			// 1: I/O port A clock enabled
	RCC->APB2ENR |= RCC_APB2ENR_AFIOEN;			// 1: Alternate Function I/O clock enabled

	// TX alternative push-pull (PA9)
	GPIOA->CRH |= GPIO_CRH_MODE9;				// 11 Output mode, max speed 50 MHz.
	GPIOA->CRH &= ~GPIO_CRH_CNF9;				// 00 CNF9 reset
	GPIOA->CRH |= GPIO_CRH_CNF9_1;				// 10 Alternate function output Push-pull

	// RX float input (PA10)
	GPIOA->CRH &= ~GPIO_CRH_MODE10;				// 00 Input mode (reset state)
	GPIOA->CRH &= ~GPIO_CRH_CNF10;				// 00 CNF10 reset
	GPIOA->CRH |= GPIO_CRH_CNF10_0;				// 01 Floating input (reset state)

	USART1->BRR = APB2_CLK / USART1_BAUD_RATE;	// USARTx_BRR = APB2CLK / USART1_BAUD_RATE
	USART1->CR1 |= USART_CR1_UE; 				// USART enable
	USART1->CR1 |= USART_CR1_TE;				// Transmitter enable
//	USART1->CR1 |= USART_CR1_RE;				// Receiver enable

//	USART1->CR1 |= USART_CR1_RXNEIE;			// RXNE interrupt enable
//	NVIC_EnableIRQ(USART1_IRQn);				// USART1 global Interrupt
}

void USART1_SendChar(char chr) {

	while (!(USART1->SR & USART_SR_TC));		// Transmission complete
	USART1->DR = chr;
}

void USART1_SendString(char *str) {

	while (*str) {
		USART1_SendChar(*str++);
	}
}

//void USART1_IRQHandler() {

//	if (USART1->SR & USART_SR_RXNE) {			// Read data register not empty
//		USART1->SR &= ~USART_SR_RXNE;			// USART_SR_RXNE reset
//
//		//USART1_SendString((char*)USART1->DR);
//	}
//}
