#include "delay.h"

#define MS_COEFF (SystemCoreClock / 1000)
#define US_COEFF (SystemCoreClock / 1000000)

// -----------------------------------------------
// SysTick Delay (only ms)
// -----------------------------------------------
#ifdef SYSTICK_DELAY_ENABLE

__IO uint32_t systick_counter = 0;

void SysTick_Init() {
	SystemCoreClockUpdate();							// Update SystemCoreClock variable
	SysTick_Config(MS_COEFF);							// 1ms
}

uint32_t SysTick_millis() {
	return systick_counter;
}

void SysTick_delay_ms(uint32_t delay) {
	uint32_t t0 = SysTick_millis();
	while ((SysTick_millis() - t0) < delay);
}

// -----------------------------------------------
// SysTick Interrupt Handler
// -----------------------------------------------
void SysTick_Handler() {
	systick_counter++;
}
#endif

// -----------------------------------------------
// DWT Init
// -----------------------------------------------
void DWT_Init(){
	CoreDebug->DEMCR |= CoreDebug_DEMCR_TRCENA_Msk;		// Enable TRC
	DWT->CYCCNT = 0;									// Reset counter
	DWT->CTRL |= DWT_CTRL_CYCCNTENA_Msk;				// Enable counter

	SystemCoreClockUpdate();							// Update SystemCoreClock variable
}

// -----------------------------------------------
// DWT Get Ticks
// -----------------------------------------------
uint32_t DWT_tick() {
	return DWT->CYCCNT;
}

// -----------------------------------------------
// DWT Get Milliseconds
// -----------------------------------------------
uint32_t DWT_millis(void) {
	return DWT_tick() / MS_COEFF;						// tick to ms
}

// -----------------------------------------------
// DWT Get Microseconds
// -----------------------------------------------
uint32_t DWT_micros(void) {
	return DWT_tick() / US_COEFF;						// tick to us
}

// -----------------------------------------------
// DWT Blocking Delay (ms)
// -----------------------------------------------
void DWT_delay_ms(uint32_t delay){
	delay *= MS_COEFF;									// ms to tick
	uint32_t t0 = DWT_tick();
	while((DWT_tick() - t0) < delay);
}

// -----------------------------------------------
// DWT Blocking Delay (us)
// -----------------------------------------------
void DWT_delay_us(uint32_t delay){
	delay *= US_COEFF;									// us to tick
	uint32_t t0 = DWT_tick();
	while((DWT_tick() - t0) < delay);
}

// -----------------------------------------------
// DWT Non Blocking Timeout (ticks)
// -----------------------------------------------
uint8_t DWT_nb_timeout(Delay_TypeDef *dt) {
	return ((DWT_tick() - dt->timestamp) > dt->delay);
}

// -----------------------------------------------
// DWT Non Blocking Delay (ms)
// -----------------------------------------------
void DWT_nb_delay_ms(Delay_TypeDef *dt, uint32_t delay) {
	dt->delay = delay * MS_COEFF;						// ms to tick
	dt->timestamp = DWT_tick();
}

// -----------------------------------------------
// DWT Non Blocking Delay (us)
// -----------------------------------------------
void DWT_nb_delay_us(Delay_TypeDef *dt, uint32_t delay) {
	dt->delay = delay * US_COEFF;						// us to tick
	dt->timestamp = DWT_tick();
}
