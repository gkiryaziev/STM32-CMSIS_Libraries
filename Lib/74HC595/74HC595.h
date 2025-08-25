/*
 * 74HC595.h
 *
 * Updated on: 09.06.2025
 * Created on: 19.05.2019
 *     Author: Admin
 *
 * LATCH    - PB12 (General output PP)
 * CLOCK    - PB13 (General output PP)
 * DATA     - PB14 (General output PP)
 *
 * NSS      - PA4 (General purpose output push-pull) (not used)
 * SCK      - PA5 (Alternate function push-pull)
 * MISO     - PA6 (Input floating / Input pull-up)   (not used)
 * MOSI     - PA7 (Alternate function push-pull)
 */

#ifndef _74HC595_H_
#define _74HC595_H_

#ifdef __cplusplus
extern "C" {
#endif /* __cplusplus */

#include "stm32f1xx.h"
#include "gpio.h"
#include "spi.h"
#include "delay.h"

#define LATCH_Port           GPIOB
#define LATCH_Pin            12
#define CLOCK_Port           GPIOB
#define CLOCK_Pin            13
#define DATA_Port            GPIOB
#define DATA_Pin             14

void HC595_SPI_Init(void);
void HC595_SPI_SendByte(uint8_t byte);
void HC595_SPI_SendBuffer(const uint8_t *data, uint8_t length);

void HC595_BitBang_Init(void);
void HC595_BitBang_SendByte(uint8_t byte);

void HC595_Latch();

#ifdef __cplusplus
}
#endif /* __cplusplus */

#endif /* _74HC595_H_ */
