/*
 * MAX6675.h
 *
 *  Created on: 25.05.2019
 *      Author: Admin
 *
 *	// SPI
 *  NSS      - PA4 (General purpose output push-pull)
 *  SCK      - PA5 (Alternate function push-pull)
 *  MISO     - PA6 (Input floating / Input pull-up)
 *  MOSI     - PA7 (Alternate function push-pull)
 *
 *	// BitBang
 *	SCK		- PB13 (General purpose output push-pull)
 *	CS		- PB14 (General purpose output push-pull)
 *	SO		- PB12 (Input floating / Input pull-up)
 */

#ifndef MAX6675_H_
#define MAX6675_H_

#ifdef __cplusplus
extern "C" {
#endif /* __cplusplus */

#include "stm32f1xx.h"
#include "gpio.h"
#include "spi.h"

#define MAX6675_SCK_Port		GPIOB
#define MAX6675_SCK_Pin			13
#define MAX6675_CS_Port			GPIOB
#define MAX6675_CS_Pin			14
#define MAX6675_SO_Port			GPIOB
#define MAX6675_SO_Pin			12

typedef enum {
	MAX6675_OK = 0x0,
	MAX6675_ERROR
} MAX6675_Status_Type;

// SPI
uint8_t MAX6675_SPI_Read(float *temperature);

// BitBang
void MAX6675_Init(void);
uint16_t MAX6675_Read(float *temperature);

#ifdef __cplusplus
}
#endif /* __cplusplus */

#endif /* MAX6675_H_ */
