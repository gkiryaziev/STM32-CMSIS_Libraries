/*
 * delay.h
 *
 *  Created on: 16.11.2018
 *      Author: Admin
 */

#ifndef DELAY_H_
#define DELAY_H_

#ifdef __cplusplus
extern "C" {
#endif /* __cplusplus */

#include "stm32f1xx.h"

// variant 1 (default)
void SysTick_Init();
uint32_t millis();
uint32_t micros();
void delay_ms(uint32_t delay);
void delay_us(uint32_t delay);

// variant 2
void G_SysTic_Init(void);
void G_delay_ms(uint32_t delay);
void G_delay_us(uint32_t delay);

// variant 3
void DWT_Init(void);
uint32_t DWT_tick(void);
void DWT_delay_ms(uint32_t delay);
void DWT_delay_us(uint32_t delay);
uint32_t DWT_millis(void);
uint32_t DWT_micros(void);

#ifdef __cplusplus
}
#endif /* __cplusplus */

#endif /* DELAY_H_ */
