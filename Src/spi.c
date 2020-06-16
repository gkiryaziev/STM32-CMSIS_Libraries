/*
 * SPI1
 * NSS : PA4 (General purpose output push-pull)
 * SCK : PA5 (Alternate function push-pull)
 * MISO: PA6 (Input floating / Input pull-up)
 * MOSI: PA7 (Alternate function push-pull)
 */
#include "spi.h"

void SPI1_Init() {

	RCC->APB2ENR |= RCC_APB2ENR_IOPAEN;			// 1: I/O port A clock enabled
	RCC->APB2ENR |= RCC_APB2ENR_AFIOEN;			// 1: Alternate Function I/O clock enabled
	RCC->APB2ENR |= RCC_APB2ENR_SPI1EN;			// 1: SPI 1 clock enabled

	// NSS
	GPIOA->CRL |= GPIO_CRL_MODE4;				// 11: Output mode, max speed 50 MHz
	GPIOA->CRL &= ~GPIO_CRL_CNF4;				// 00: General purpose output push-pull
	// SCK
	GPIOA->CRL |= GPIO_CRL_MODE5;				// 11: Output mode, max speed 50 MHz.
	GPIOA->CRL &= ~GPIO_CRL_CNF5;				// Reset
	GPIOA->CRL |= GPIO_CRL_CNF5_1;				// 10: Alternate function output Push-pull
	// MISO
	GPIOA->CRL &= ~GPIO_CRL_MODE6;				// 00: Input mode (reset state)
	GPIOA->CRL &= ~GPIO_CRL_CNF6;				// Reset
	GPIOA->CRL |= GPIO_CRL_CNF6_0;				// 01: Floating input (reset state)
	// MOSI
	GPIOA->CRL |= GPIO_CRL_MODE7;				// 11: Output mode, max speed 50 MHz.
	GPIOA->CRL &= ~GPIO_CRL_CNF7;				// Reset
	GPIOA->CRL |= GPIO_CRL_CNF7_1;				// 10: Alternate function output Push-pull

	// SPI1
	SPI1->CR1 = 0;								// Reset
	SPI1->CR1 |= SPI_CR1_MSTR;					// 1: Master configuration
	SPI1->CR1 |= SPI_CR1_BR_2 | SPI_CR1_BR_1;	// 110: fPCLK/128
	SPI1->CR1 |= SPI_CR1_SSI;					// SSI: Internal slave select
	SPI1->CR1 |= SPI_CR1_SSM;					// 1: Software slave management enabled

	#ifdef SPI1_16_BIT_FORMAT
	SPI1->CR1 |= SPI_CR1_DFF;					// 1: 16-bit data frame format is selected for transmission/reception
	#endif

	SPI1->CR1 |= SPI_CR1_SPE;					// 1: Peripheral enabled

	SPI1_DisableSlave();
}

void SPI1_EnableSlave() {

	GPIOA->BRR |= GPIO_BRR_BR4;					// 1: Reset the corresponding ODRx bit
}

void SPI1_DisableSlave() {

	GPIOA->BSRR |= GPIO_BSRR_BS4;				// 1: Set the corresponding ODRx bit
}

uint16_t SPI1_Write(uint16_t data) {

	SPI1->DR = data;

	while (!(SPI1->SR & SPI_SR_TXE));			// 0: Tx buffer not empty
	while (!(SPI1->SR & SPI_SR_RXNE));			// 0: Rx buffer empty
	while (SPI1->SR & SPI_SR_BSY);				// 1: SPI (or I2S) is busy in communication or Tx buffer is not empty

	return (uint16_t)SPI1->DR;
}
