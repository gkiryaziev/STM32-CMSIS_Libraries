#include "delay.h"

static __IO uint8_t _variant = 0;			// variants, 0 - default
static __IO uint32_t delay_counter = 0;		// variant 1
static __IO uint32_t sysTickCounter = 0;	// variant 2

// -----------------------------------------------
// variant 1
// -----------------------------------------------
void SysTick_Init() {
	_variant = 0;
	SysTick_Config(SystemCoreClock / 1000);
}

uint32_t millis() {
	return delay_counter;
}

uint32_t micros() {
	return millis() * 1000 + 1000 - SysTick->VAL / 72; // 72 = (SystemCoreClock / 1000000)
}

void delay_ms(uint32_t delay) {
	uint32_t start = millis();
	while ((millis() - start) < delay) {
	}
}

void delay_us(uint32_t delay) {
	uint32_t start = micros();
	while ((micros() - start) < delay) {
	}
}

// -----------------------------------------------
// variant 2
// -----------------------------------------------
/******************************************
 *SystemFrequency/1000        1ms         *
 *SystemFrequency/100000      10us        *
 *SystemFrequency/1000000.0   1us         *
 ******************************************/
void G_SysTic_Init() {
	_variant = 1;
	SystemCoreClockUpdate();
	while (SysTick_Config(SystemCoreClock / 1000000.0) != 0) {
	}
}

void G_delay_ms(uint32_t delay) {
	while (delay--) {
		G_delay_us(1000);
	}
}

void G_delay_us(uint32_t delay) {
	sysTickCounter = delay;
	while (sysTickCounter != 0);
}

// Interrupt Handler
void SysTick_Handler() {
	if (_variant == 0) {
		delay_counter++;			// variant 1
	} else if (_variant == 1) {
		if (sysTickCounter != 0) {	// variant 2
			sysTickCounter--;
		}
	} else {}						// variant 3
}

// -----------------------------------------------
// variant 3
// -----------------------------------------------
void DWT_Init(){
	CoreDebug->DEMCR |= CoreDebug_DEMCR_TRCENA_Msk;		// Enable TRC
	DWT->CYCCNT = 0;									// Reset counter
	DWT->CTRL |= DWT_CTRL_CYCCNTENA_Msk;				// Enable counter
}

uint32_t DWT_tick() {
	return DWT->CYCCNT;
}

void DWT_delay_ms(uint32_t delay){
	DWT_delay_us(delay * 1000);
}

void DWT_delay_us(uint32_t delay){
	uint32_t t0 = DWT->CYCCNT;
	delay *= (SystemCoreClock / 1000000);
	while((DWT->CYCCNT - t0) < delay);
}

uint32_t DWT_millis(void) {
	return DWT_tick() / (SystemCoreClock / 1000);
}

uint32_t DWT_micros(void) {
	return DWT_tick() / (SystemCoreClock / 1000000 + 1);
}
