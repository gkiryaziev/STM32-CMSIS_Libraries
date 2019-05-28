#include "gpio.h"

void GPIO_Init() {

	GPIO_Disable_JTAG();									// Remap PA15, PB3, PB4 as GPIO

	// PC13
	GPIO_Enable(GPIOC);
	GPIO_SetMode_Output_2MHz_PP(GPIOC, 13);
	GPIO_PC13_Off();
}

void GPIO_SetPin(GPIO_TypeDef *GPIOx, uint8_t pin) {

	GPIOx->BSRR |= (1 << pin);								// 1: Set the corresponding ODRx bit
}

void GPIO_ResetPin(GPIO_TypeDef *GPIOx, uint8_t pin) {

	GPIOx->BRR |= (1 << pin);								// 1: Reset the corresponding ODRx bit
}

// -----------------------------------
// GPIOx GPIOA, GPIOB, GPIOC
// pin   0 - 15
// state 0, 1
// -----------------------------------
void GPIO_WritePin(GPIO_TypeDef *GPIOx, uint8_t pin, uint8_t state) {

	if (state) {
		GPIO_SetPin(GPIOx, pin);
	} else {
		GPIO_ResetPin(GPIOx, pin);
	}
}

uint16_t GPIO_ReadPin(GPIO_TypeDef *GPIOx, uint8_t pin) {

	return (GPIOx->IDR & (1 << pin));
}

void GPIO_Enable(GPIO_TypeDef *GPIOx) {

	if (GPIOx == GPIOA) RCC->APB2ENR |= RCC_APB2ENR_IOPAEN;	// 1: IO port A clock enabled
	if (GPIOx == GPIOB) RCC->APB2ENR |= RCC_APB2ENR_IOPBEN;	// 1: IO port B clock enabled
	if (GPIOx == GPIOC) RCC->APB2ENR |= RCC_APB2ENR_IOPCEN;	// 1: IO port C clock enabled
}

void GPIO_Disable_JTAG(void) {

	RCC->APB2ENR |= RCC_APB2ENR_AFIOEN;						// 1: Alternate Function I/O clock enabled
	AFIO->MAPR |= AFIO_MAPR_SWJ_CFG_JTAGDISABLE;			// 010: JTAG-DP Disabled and SW-DP Enabled
}

// -----------------------------------------
// General purpose output push-pull, 2MHz
// -----------------------------------------
void GPIO_SetMode_Output_2MHz_PP(GPIO_TypeDef *GPIOx, uint8_t pin) {

	if (pin <= 7) {											// CRL
		GPIOx->CRL &= ~(3 << (pin * 4));					// MODE reset
		GPIOx->CRL |= (2 << (pin * 4));						// 10: Output mode, max speed 2 MHz.
		GPIOx->CRL &= ~(3 << ((pin * 4) + 2));				// 00: General purpose output push-pull
	} else {												// CRH
		GPIOx->CRH &= ~(3 << ((pin - 8) * 4));				// MODE reset
		GPIOx->CRH |= (2 << ((pin - 8) * 4));				// 10: Output mode, max speed 2 MHz.
		GPIOx->CRH &= ~(3 << (((pin - 8) * 4) + 2));		// 00: General purpose output push-pull
	}
}

// -----------------------------------------
// General purpose output push-pull, 50MHz
// -----------------------------------------
void GPIO_SetMode_Output_50MHz_PP(GPIO_TypeDef *GPIOx, uint8_t pin) {

	if (pin <= 7) { 										// CRL
		GPIOx->CRL |= (3 << (pin * 4));						// 11: Output mode, max speed 50 MHz
		GPIOx->CRL &= ~(3 << ((pin * 4) + 2));				// 00: General purpose output push-pull
	} else {												// CRH
		GPIOx->CRH |= (3 << ((pin - 8) * 4));				// 11: Output mode, max speed 50 MHz
		GPIOx->CRH &= ~(3 << (((pin - 8) * 4) + 2));		// 00: General purpose output push-pull
	}
}

// -----------------------------------------
// Floating input (reset state)
// -----------------------------------------
void GPIO_SetMode_Input_Floating(GPIO_TypeDef *GPIOx, uint8_t pin) {

	if (pin <= 7) { 										// CRL
		GPIOx->CRL &= ~(3 << (pin * 4));					// 00: Input mode (reset state)
		GPIOx->CRL &= ~(3 << ((pin * 4) + 2));				// CNF reset
		GPIOx->CRL |= (1 << ((pin * 4) + 2)); 				// 01: Floating input (reset state)
	} else {												// CRH
		GPIOx->CRH &= ~(3 << ((pin - 8) * 4));				// 00: Input mode (reset state)
		GPIOx->CRH &= ~(3 << (((pin - 8) * 4) + 2));		// CNF reset
		GPIOx->CRH |= (1 << (((pin - 8) * 4) + 2));			// 01: Floating input (reset state)
	}
}

void GPIO_PC13_On() {

	GPIO_WritePin(GPIOC, 13, 0);
}

void GPIO_PC13_Off() {

	GPIO_WritePin(GPIOC, 13, 1);
}
