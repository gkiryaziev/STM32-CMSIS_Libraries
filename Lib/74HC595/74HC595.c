/*
 * 74HC595.c
 *
 *  Created on: 19.05.2019
 *      Author: Admin
 *
 *	LATCH    - PB12 (General output PP)
 *  CLOCK    - PB13 (General output PP)
 *  DATA     - PB14 (General output PP)
 *
 *	NSS      - PA4 (General purpose output push-pull)	(not used)
 *  SCK      - PA5 (Alternate function push-pull)
 *  MISO     - PA6 (Input floating / Input pull-up)		(not used)
 *  MOSI     - PA7 (Alternate function push-pull)
 */

#include "74HC595.h"

void HC595_SPI_Init(void) {

	GPIO_Enable(LATCH_Port);							// Enable port
	GPIO_SetMode_Output_2MHz_PP(LATCH_Port, LATCH_Pin);	// Init latch pin
	GPIO_WritePin(LATCH_Port, LATCH_Pin, 0);			// Low level
}

void HC595_SPI_SendByte(uint8_t byte) {

	uint16_t counter = 1000;

	SPI1_Write(byte);

	// Toggle latch pin
	GPIO_WritePin(LATCH_Port, LATCH_Pin, 1);
	while (counter--);
	GPIO_WritePin(LATCH_Port, LATCH_Pin, 0);
}


void HC595_Init(void){

	GPIO_Enable(LATCH_Port);

	GPIO_SetMode_Output_2MHz_PP(CLOCK_Port, CLOCK_Pin);
	GPIO_SetMode_Output_2MHz_PP(LATCH_Port, LATCH_Pin);
	GPIO_SetMode_Output_2MHz_PP(DATA_Port, DATA_Pin);

	GPIO_WritePin(CLOCK_Port, CLOCK_Pin, 0);
	GPIO_WritePin(LATCH_Port, LATCH_Pin, 0);
	GPIO_WritePin(DATA_Port, DATA_Pin, 0);
}

void HC595_SendByte(uint8_t byte){

	uint16_t clock_timeout = 10000;
	uint16_t latch_timeout = 1000;

	for (int8_t i = 7; i >= 0; --i) {

		uint8_t bit = byte & (0x1 << i);				// Read bit
		GPIO_WritePin(DATA_Port, DATA_Pin, bit);		// Send bit

		// Toggle clock
		GPIO_WritePin(CLOCK_Port, CLOCK_Pin, 1);
		while(clock_timeout--);
		GPIO_WritePin(CLOCK_Port, CLOCK_Pin, 0);
	}

	// Toggle latch
	GPIO_WritePin(LATCH_Port, LATCH_Pin, 1);
	while (latch_timeout--);
	GPIO_WritePin(LATCH_Port, LATCH_Pin, 0);
}
