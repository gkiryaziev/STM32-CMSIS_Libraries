#include "rcc.h"

void RCC_Init() {
	RCC->CR |= RCC_CR_HSEON;												// HSEON: HSE clock enable, 1: HSE oscillator ON
	while ((RCC->CR & RCC_CR_HSERDY) == 0);									// HSERDY: External high-speed clock ready flag, 1: HSE oscillator ready

	FLASH->ACR = FLASH_ACR_PRFTBE | FLASH_ACR_LATENCY_1;					// 1: Prefetch is enabled, 010 Two wait states, if 48 MHz < SYSCLK < 72 MHz

	RCC->CFGR |= RCC_CFGR_HPRE_DIV1;										// HPRE: AHB prescaler, 0xxx: SYSCLK not divided
	RCC->CFGR |= RCC_CFGR_PPRE1_DIV2;										// PPRE1: APB low-speed prescaler (APB1), 100: HCLK divided by 2
	RCC->CFGR |= RCC_CFGR_PPRE2_DIV1;										// PPRE2: APB high-speed prescaler (APB2), 0xx: HCLK not divided

	RCC->CFGR &= ~(RCC_CFGR_PLLSRC | RCC_CFGR_PLLXTPRE | RCC_CFGR_PLLMULL);	// Reset PLL
	RCC->CFGR |= RCC_CFGR_PLLXTPRE_HSE;										// PLLXTPRE: HSE divider for PLL entry, 0: HSE clock not divided
	RCC->CFGR |= RCC_CFGR_PLLSRC;											// PLLSRC: PLL entry clock source, 1: HSE oscillator clock selected as PLL input clock
	RCC->CFGR |= RCC_CFGR_PLLMULL9;											// PLLMUL: PLL multiplication factor, 0111: PLL input clock x 9

	RCC->CR |= RCC_CR_PLLON;												// PLLON: PLL enable, 1: PLL ON
	while ((RCC->CR & RCC_CR_PLLRDY) == 0);									// PLLRDY: PLL clock ready flag, 1: PLL locked

	RCC->CFGR &= ~RCC_CFGR_SW;												// SW: System clock switch, 00: HSI selected as system clock
	RCC->CFGR |= RCC_CFGR_SW_PLL;											// SW: System clock switch, 10: PLL selected as system clock
	while ((RCC->CFGR & RCC_CFGR_SWS) != RCC_CFGR_SWS_PLL);					// SWS: System clock switch status, 10: PLL selected as system clock

	SystemCoreClockUpdate();
}
