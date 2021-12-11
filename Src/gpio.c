#include "gpio.h"

// -----------------------------------------
// GPIO Init
// -----------------------------------------
void GPIO_Init() {

	GPIO_Disable_JTAG();									// Remap PA15, PB3, PB4 as GPIO

	// Blue Pill Led, PC13
	GPIO_Enable(GPIOC);
	GPIO_SetMode_Output_50MHz_PP(GPIOC, 13);
	GPIO_PC13_Off();
}

// -----------------------------------------
// GPIOx: GPIOA, GPIOB, GPIOC
// pin  : 0 - 15
// -----------------------------------------
void GPIO_SetPin(GPIO_TypeDef *GPIOx, uint8_t pin) {

	GPIOx->BSRR |= (1 << pin);								// 1: Set the corresponding ODRx bit
}

// -----------------------------------------
// GPIOx: GPIOA, GPIOB, GPIOC
// pin  : 0 - 15
// -----------------------------------------
void GPIO_ResetPin(GPIO_TypeDef *GPIOx, uint8_t pin) {

	GPIOx->BRR |= (1 << pin);								// 1: Reset the corresponding ODRx bit
}

// -----------------------------------
// GPIOx: GPIOA, GPIOB, GPIOC
// pin  : 0 - 15
// state: 0, 1
// -----------------------------------
void GPIO_WritePin(GPIO_TypeDef *GPIOx, uint8_t pin, uint8_t state) {

	if (state) {
		GPIO_SetPin(GPIOx, pin);
	} else {
		GPIO_ResetPin(GPIOx, pin);
	}
}

// -----------------------------------------
// GPIOx: GPIOA, GPIOB, GPIOC
// pin  : 0 - 15
// -----------------------------------------
uint16_t GPIO_ReadPin(GPIO_TypeDef *GPIOx, uint8_t pin) {

	return (GPIOx->IDR & (1 << pin));
}

// -----------------------------------------
// GPIOx: GPIOA, GPIOB, GPIOC
// -----------------------------------------
void GPIO_Enable(GPIO_TypeDef *GPIOx) {

	if (GPIOx == GPIOA) RCC->APB2ENR |= RCC_APB2ENR_IOPAEN;	// 1: IO port A clock enabled
	if (GPIOx == GPIOB) RCC->APB2ENR |= RCC_APB2ENR_IOPBEN;	// 1: IO port B clock enabled
	if (GPIOx == GPIOC) RCC->APB2ENR |= RCC_APB2ENR_IOPCEN;	// 1: IO port C clock enabled
}

// -----------------------------------------
// JTAG-DP Disabled and SW-DP Enabled
// -----------------------------------------
void GPIO_Disable_JTAG(void) {

	RCC->APB2ENR |= RCC_APB2ENR_AFIOEN;						// 1: Alternate Function I/O clock enabled
	AFIO->MAPR |= AFIO_MAPR_SWJ_CFG_JTAGDISABLE;			// 010: JTAG-DP Disabled and SW-DP Enabled
}

// -----------------------------------------
// General purpose output push-pull, 2MHz
// GPIOx: GPIOA, GPIOB, GPIOC
// pin  : 0 - 15
// -----------------------------------------
void GPIO_SetMode_Output_2MHz_PP(GPIO_TypeDef *GPIOx, uint8_t pin) {

	if (pin <= 7) {											// CRL
		GPIOx->CRL &= ~(0xF << (pin * 4));					// 0000: [CNF, MODE] reset
		GPIOx->CRL |= (0x2 << (pin * 4));					// 0010: General purpose output push-pull, max speed 2 MHz
	} else {												// CRH
		GPIOx->CRH &= ~(0xF << ((pin - 8) * 4));			// 0000: [CNF, MODE] reset
		GPIOx->CRH |= (0x2 << ((pin - 8) * 4));				// 0010: General purpose output push-pull, max speed 2 MHz
	}
}

// -----------------------------------------
// General purpose output push-pull, 10MHz
// GPIOx: GPIOA, GPIOB, GPIOC
// pin  : 0 - 15
// -----------------------------------------
void GPIO_SetMode_Output_10MHz_PP(GPIO_TypeDef *GPIOx, uint8_t pin) {

	if (pin <= 7) {											// CRL
		GPIOx->CRL &= ~(0xF << (pin * 4));					// 0000: [CNF, MODE] reset
		GPIOx->CRL |= (0x1 << (pin * 4));					// 0001: General purpose output push-pull, max speed 10 MHz
	} else {												// CRH
		GPIOx->CRH &= ~(0xF << ((pin - 8) * 4));			// 0000: [CNF, MODE] reset
		GPIOx->CRH |= (0x1 << ((pin - 8) * 4));				// 0001: General purpose output push-pull, max speed 10 MHz
	}
}

// -----------------------------------------
// General purpose output push-pull, 50MHz
// GPIOx: GPIOA, GPIOB, GPIOC
// pin  : 0 - 15
// -----------------------------------------
void GPIO_SetMode_Output_50MHz_PP(GPIO_TypeDef *GPIOx, uint8_t pin) {

	if (pin <= 7) { 										// CRL
		GPIOx->CRL &= ~(0xF << (pin * 4));					// 0000: [CNF, MODE] reset
		GPIOx->CRL |= (0x3 << (pin * 4));					// 0011: General purpose output push-pull, max speed 50 MHz
	} else {												// CRH
		GPIOx->CRH &= ~(0xF << ((pin - 8) * 4));			// 0000: [CNF, MODE] reset
		GPIOx->CRH |= (0x3 << ((pin - 8) * 4));				// 0011: General purpose output push-pull, max speed 50 MHz
	}
}

// -----------------------------------------
// Floating input (reset state)
// GPIOx: GPIOA, GPIOB, GPIOC
// pin  : 0 - 15
// -----------------------------------------
void GPIO_SetMode_Input_Floating(GPIO_TypeDef *GPIOx, uint8_t pin) {

	if (pin <= 7) { 										// CRL
		GPIOx->CRL &= ~(0xF << (pin * 4));					// 0000: [CNF, MODE] reset
		GPIOx->CRL |= (0x4 << (pin * 4));					// 0100: Input mode, floating (reset state)
	} else {												// CRH
		GPIOx->CRH &= ~(0xF << ((pin - 8) * 4));			// 0000: [CNF, MODE] reset
		GPIOx->CRH |= (0x4 << ((pin - 8) * 4));				// 0100: Input mode, floating (reset state)
	}
}

// -----------------------------------------
// Input pull-down
// GPIOx: GPIOA, GPIOB, GPIOC
// pin  : 0 - 15
// -----------------------------------------
void GPIO_SetMode_Input_PullDown(GPIO_TypeDef *GPIOx, uint8_t pin) {

	if (pin <= 7) { 										// CRL
		GPIOx->CRL &= ~(0xF << (pin * 4));					// 0000: [CNF, MODE] reset
		GPIOx->CRL |= (0x8 << (pin * 4));					// 1000: Input with pull-up / pull-down
	} else {												// CRH
		GPIOx->CRH &= ~(0xF << ((pin - 8) * 4));			// 0000: [CNF, MODE] reset
		GPIOx->CRH |= (0x8 << ((pin - 8) * 4));				// 1000: Input with pull-up / pull-down
	}

	GPIOx->ODR &= ~(0x1 << pin);							// Port output data (y= 0 .. 15)
}

// -----------------------------------------
// Input pull-up
// GPIOx: GPIOA, GPIOB, GPIOC
// pin  : 0 - 15
// -----------------------------------------
void GPIO_SetMode_Input_PullUp(GPIO_TypeDef *GPIOx, uint8_t pin) {

	if (pin <= 7) { 										// CRL
		GPIOx->CRL &= ~(0xF << (pin * 4));					// 0000: [CNF, MODE] reset
		GPIOx->CRL |= (0x8 << (pin * 4));					// 1000: Input with pull-up / pull-down
	} else {												// CRH
		GPIOx->CRH &= ~(0xF << ((pin - 8) * 4));			// 0000: [CNF, MODE] reset
		GPIOx->CRH |= (0x8 << ((pin - 8) * 4));				// 1000: Input with pull-up / pull-down
	}

	GPIOx->ODR |= (0x1 << pin);								// Port output data (y= 0 .. 15)
}

// -----------------------------------------
// Blue Pill Led, PC13, Inverted, 0 is On
// -----------------------------------------
void GPIO_PC13_On() {

	GPIO_WritePin(GPIOC, 13, 0);
}

// -----------------------------------------
// Blue Pill Led, PC13, Inverted, 1 is Off
// -----------------------------------------
void GPIO_PC13_Off() {

	GPIO_WritePin(GPIOC, 13, 1);
}
