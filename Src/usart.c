/*
 * USART1
 * APB CLOCK: 72MHz
 * BAUD RATE: 115200
 * TX       : PA9
 * RX       : PA10
 */

#include "usart.h"

volatile char usart1_buffer[USART1_BUFFER_SIZE] = {0};
volatile uint8_t usart1_index = 0;
volatile uint8_t usart1_ready = 0;

// -----------------------------------------------
// USART1 Init
// -----------------------------------------------
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
	USART1->CR1 |= USART_CR1_RE;				// Receiver enable

	USART1->CR1 |= USART_CR1_RXNEIE;			// RXNE interrupt enable
	NVIC_EnableIRQ(USART1_IRQn);				// USART1 global Interrupt
}

// -----------------------------------------------
// USART1 Send Char
// -----------------------------------------------
void USART1_SendChar(char ch) {

	while (!(USART1->SR & USART_SR_TXE));		// 0: Data is not transferred to the shift register
	USART1->SR &= ~USART_SR_TC;					// USART_SR_TC clear. This clearing sequence is recommended only for multibuffer communication.
	
	USART1->DR = ch;
}

// -----------------------------------------------
// USART1 Send String
// -----------------------------------------------
void USART1_SendString(char *str) {

	while (*str) {
		USART1_SendChar(*str++);
	}
}

// -----------------------------------------------
// USART1 Read String, copy buffer
// -----------------------------------------------
uint8_t USART1_ReadString(char *str) {

	if (usart1_ready) {

		uint8_t i = 0;

		while (usart1_buffer[i] != 0) {
			str[i] = usart1_buffer[i];		// Copy volatile buffer to external buffer
			i++;
		}

		str[i] = 0;							// Add terminating NULL to external buffer
		usart1_ready = 0;					// Read usart buffer only once

		return 1;
	}

	return 0;
}

// -----------------------------------------------
// USART1 Read String, return buffer
// Experimental because of warning:
// warning: return discards 'volatile' qualifier from pointer target type
// -----------------------------------------------
char *USART1_ReadStringExp() {

	if (usart1_ready) {
		usart1_ready = 0;					// Read usart buffer only once

		return usart1_buffer;
	}

	return 0;
}

// -----------------------------------------------
// USART1 IRQHandler
// -----------------------------------------------
void USART1_IRQHandler() {

	if (USART1->SR & USART_SR_RXNE) {		// 1: Received data is ready to be read
		USART1->SR &= ~USART_SR_RXNE;		// USART_SR_RXNE clear. This clearing sequence is recommended only for multibuffer communication.

		char rx = (char)USART1->DR;

		if ((rx == '\r') || (rx == '\n')) {
			usart1_buffer[usart1_index] = 0;			// Add terminating NULL
			usart1_index = 0;
			usart1_ready = 1;
		} else {
			if (usart1_index == USART1_BUFFER_SIZE - 1)	// If overflows, reset index
				usart1_index = 0;

			usart1_buffer[usart1_index++] = rx;
			usart1_ready = 0;							// New data, reset ready flag
		}
	}
}
