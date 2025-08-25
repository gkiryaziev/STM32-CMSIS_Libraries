/*
 * delay.c
 *
 * Updated on: 28.05.2025
 * Updated on: 03.06.2020
 * Created on: 16.11.2018
 *     Author: Admin
 */

#include "delay.h"

static uint32_t ms_coeff = 0;
static uint32_t us_coeff = 0;


// -----------------------------------------------
// SysTick Delay (only ms)
// -----------------------------------------------
#ifdef SYSTICK_DELAY_ENABLE

volatile uint32_t systick_counter = 0;

void SysTick_Init() {
    SystemCoreClockUpdate();                            // Update SystemCoreClock variable
    ms_coeff = SystemCoreClock / 1000;
    SysTick_Config(ms_coeff);                           // 1ms
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
// DWT_Init
// -----------------------------------------------
void DWT_Init(void) {
    SystemCoreClockUpdate();                            // Update SystemCoreClock variable

    CoreDebug->DEMCR |= CoreDebug_DEMCR_TRCENA_Msk;     // Enable TRC
    DWT->CYCCNT = 0;                                    // Reset counter
    DWT->CTRL |= DWT_CTRL_CYCCNTENA_Msk;                // Enable counter

    ms_coeff = SystemCoreClock / 1000;
    us_coeff = SystemCoreClock / 1000000;
}

// -----------------------------------------------
// DWT Get Ticks
// -----------------------------------------------
uint32_t DWT_tick(void) {
    return DWT->CYCCNT;
}

// -----------------------------------------------
// DWT Get Milliseconds
// -----------------------------------------------
uint32_t DWT_millis(void) {
    return DWT_tick() / ms_coeff;                       // tick to ms
}

// -----------------------------------------------
// DWT Get Microseconds
// -----------------------------------------------
uint32_t DWT_micros(void) {
    return DWT_tick() / us_coeff;                       // tick to us
}

// -----------------------------------------------
// DWT Blocking Delay (ms)
// -----------------------------------------------
void DWT_delay_ms(uint32_t delay) {
    delay *= ms_coeff;                                  // ms to tick
    uint32_t t0 = DWT_tick();
    while((DWT_tick() - t0) < delay);
}

// -----------------------------------------------
// DWT Blocking Delay (us)
// -----------------------------------------------
void DWT_delay_us(uint32_t delay) {
    delay *= us_coeff;                                  // us to tick
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
    dt->delay = delay * ms_coeff;                       // ms to tick
    dt->timestamp = DWT_tick();
}

// -----------------------------------------------
// DWT Non Blocking Delay (us)
// -----------------------------------------------
void DWT_nb_delay_us(Delay_TypeDef *dt, uint32_t delay) {
    dt->delay = delay * us_coeff;                       // us to tick
    dt->timestamp = DWT_tick();
}
