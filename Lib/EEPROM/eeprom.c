/*
 * eeprom.c
 *
 *  Created on: 23 мая 2019 г.
 *      Author: Admin
 */

#include "eeprom.h"

uint8_t EEPROM_GetStatus() {

	// I2C Send
	I2C1_Start();
	if (I2C1_SendAddress(EEPROM_ADDRESS, I2C_TRANSMITTER) == I2C_ERROR) return 0;
	I2C1_Stop();

	return 1;
}

void EEPROM_WriteByte(uint16_t address, uint8_t data) {

	uint8_t temp[] = { data };

	EEPROM_WritePage(address, temp, 1);
}

void EEPROM_WritePage(uint16_t address, uint8_t *data, uint8_t size) {

	uint8_t msb = (address >> 8) & 0xFF;
	uint8_t lsb = address & 0xFF;

	// I2C Send
	I2C1_Start();
	I2C1_SendAddress(EEPROM_ADDRESS, I2C_TRANSMITTER);
	I2C1_SendData(msb);
	I2C1_SendData(lsb);
	for (uint8_t i = 0; i < size; ++i) {
		I2C1_SendData(data[i]);
	}
	I2C1_Stop();

	DWT_delay_ms(15);		// [tWR] Write Cycle Time (MAX) = 10ms
}

uint8_t EEPROM_ReadByte(uint16_t address) {

	uint8_t temp[1] = {0};

	EEPROM_ReadPage(address, temp, 1);

	return temp[0];
}

void EEPROM_ReadPage(uint16_t address, uint8_t *data, uint8_t size) {

	uint8_t msb = (address >> 8) & 0xFF;
	uint8_t lsb = address & 0xFF;

	uint8_t counter = 0;

	// I2C1 Send
	I2C1_Start();
	I2C1_SendAddress(EEPROM_ADDRESS, I2C_TRANSMITTER);
	I2C1_SendData(msb);
	I2C1_SendData(lsb);
	I2C1_Start();
	I2C1_SendAddress(EEPROM_ADDRESS, I2C_RECEIVER);

	// I2C1 Receive
	if (size > 1) {
		for (uint8_t i = 0; i < size - 1; ++i) {
			data[counter] = I2C1_ReceiveData(I2C_ACK);
			counter++;
		}
	}

	I2C1_Stop();
	data[counter] = I2C1_ReceiveData(I2C_NACK);
}
