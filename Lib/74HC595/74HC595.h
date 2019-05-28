/*
 * 74HC595.h
 *
 *  Created on: 19.05.2019
 *      Author: Admin
 */

#ifndef _74HC595_H_
#define _74HC595_H_

#ifdef __cplusplus
extern "C" {
#endif /* __cplusplus */

#include "stm32f1xx.h"
#include "gpio.h"
#include "spi.h"

#define LATCH_Port			GPIOB
#define LATCH_Pin			12
#define CLOCK_Port			GPIOB
#define CLOCK_Pin			13
#define DATA_Port			GPIOB
#define DATA_Pin			14

void HC595_SPI_Init(void);
void HC595_SPI_SendByte(uint8_t byte);

void HC595_Init(void);
void HC595_SendByte(uint8_t byte);

#ifdef __cplusplus
}
#endif /* __cplusplus */

#endif /* _74HC595_H_ */
