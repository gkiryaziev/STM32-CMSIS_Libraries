/*
 * PCF8574T.c
 *
 *  Created on: 25.05.2019
 *      Author: Admin
 */

#include "PCF8574T.h"

volatile uint8_t PCF8574T_Port = 0xFF;

void PCF8574T_Init(void) {}

uint8_t PCF8574T_GetStatus(void) {

	// I2C Send
	I2C1_Start();
	if (I2C1_SendAddress(PCF8574T_ADDRESS, I2C_TRANSMITTER) == I2C_ERROR) return 0;
	I2C1_Stop();

	return 1;
}

uint8_t PCF8574T_Read() {

	// I2C Receive
	I2C1_Start();
	I2C1_SendAddress(PCF8574T_ADDRESS, I2C_RECEIVER);
	I2C1_Stop();

	return I2C1_ReceiveData(I2C_NACK);
}

void PCF8574T_Write(uint8_t data) {

	// I2C Send
	I2C1_Start();
	I2C1_SendAddress(PCF8574T_ADDRESS, I2C_TRANSMITTER);
	I2C1_SendData(data);
	I2C1_Stop();
}

uint8_t PCF8574T_ReadPin(uint8_t pin) {

	PCF8574T_Port = PCF8574T_Read();

	return (PCF8574T_Port & (1 << pin)) ? 1 : 0;
}

void PCF8574T_WritePin(uint8_t pin, uint8_t state) {

	// Read current state of port.
	// PCF8574T_Port = PCF8574T_Read();

	if (state) {
		PCF8574T_Port |= (1 << pin);
	} else {
		PCF8574T_Port &= ~(1 << pin);
	}

	PCF8574T_Write(PCF8574T_Port);
}
