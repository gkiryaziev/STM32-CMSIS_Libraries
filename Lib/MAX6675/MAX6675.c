/*
 * MAX6675.c
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

#include "MAX6675.h"

uint8_t MAX6675_SPI_Read(float *temperature) {

	uint16_t temp_val = 0;

	SPI1_EnableSlave();
	temp_val = SPI1_Write(0x0);
	temp_val <<= 8;
	temp_val |= SPI1_Write(0x0);
	SPI1_DisableSlave();

	*temperature = (float)(temp_val >> 3) * 0.25;

	return ((temp_val >> 2) & 0x1);
}

void MAX6675_Init() {

	GPIO_Enable(GPIOB);
	GPIO_SetMode_Output_50MHz_PP(MAX6675_SCK_Port, MAX6675_SCK_Pin);
	GPIO_SetMode_Output_50MHz_PP(MAX6675_CS_Port, MAX6675_CS_Pin);
	GPIO_SetMode_Input_Floating(MAX6675_SO_Port, MAX6675_SO_Pin);
	GPIO_WritePin(MAX6675_CS_Port, MAX6675_CS_Pin, 1);				// CS high
	GPIO_WritePin(MAX6675_SCK_Port, MAX6675_SCK_Pin, 0);			// SCK low
}

uint16_t MAX6675_Read(float *temperature) {

	uint16_t timeout = 1000;
	uint16_t temp_val = 0;

	GPIO_WritePin(MAX6675_CS_Port, MAX6675_CS_Pin, 0);				// CS low

	for (int8_t i = 15; i >= 0; --i) {

		GPIO_WritePin(MAX6675_SCK_Port, MAX6675_SCK_Pin, 1);		// SCK high

		while(timeout--);

		if (GPIO_ReadPin(MAX6675_SO_Port, MAX6675_SO_Pin)) {
			temp_val |= (1 << i);
		}

		GPIO_WritePin(MAX6675_SCK_Port, MAX6675_SCK_Pin, 0);		// SCK low
	}

	GPIO_WritePin(MAX6675_CS_Port, MAX6675_CS_Pin, 1);				// CS high

	*temperature = (float)(temp_val >> 3) * 0.25;

	return ((temp_val >> 2) & 0x1);
}
