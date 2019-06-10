#include "ADS1115.h"

// ---------------------------------------------
// ADS1115_ReadRegisters
// ---------------------------------------------
void ADS1115_ReadRegisters(uint8_t address, uint8_t *data, uint8_t size) {

	uint8_t count = 0;

	// I2C Send
	I2C1_Start();
	I2C1_SendAddress(ADS1115_ADDRESS, I2C_TRANSMITTER);
	I2C1_SendData(address);

	// I2C Receive
	I2C1_Start();
	I2C1_SendAddress(ADS1115_ADDRESS, I2C_RECEIVER);

	for (count = 0; count < size - 1; ++count ) {
		data[count] = I2C1_ReceiveData(I2C_ACK);
	}

	I2C1_Stop();
	data[count] = I2C1_ReceiveData(I2C_NACK);
}

// ---------------------------------------------
// ADS1115_WriteRegisters
// ---------------------------------------------
void ADS1115_WriteRegisters(uint8_t address, uint8_t *data, uint8_t size) {

	// I2C Send
	I2C1_Start();
	I2C1_SendAddress(ADS1115_ADDRESS, I2C_TRANSMITTER);
	I2C1_SendData(address);

	for (int8_t i = 0; i < size; ++i) {
		I2C1_SendData(data[i]);
	}

	I2C1_Stop();
}

void ADS1115_Init(void) {}

uint8_t ADS1115_GetStatus(void) {

	// I2C Send
	I2C1_Start();
	if (I2C1_SendAddress(ADS1115_ADDRESS, I2C_TRANSMITTER) == I2C_ERROR) return 0;
	I2C1_Stop();

	return 1;
}

uint16_t ADS1115_ReadChannel(uint8_t channel) {

	if (channel > 3) {
		return 0;
	}

	uint16_t register_data = 0;
	int16_t received_data = 0;
	uint8_t data[2] = {0};

	register_data = ADS1115_COMP_QUE;
	register_data &= ~(ADS1115_COMP_LAT);
	register_data &= ~(ADS1115_COMP_POL);
	register_data &= ~(ADS1115_COMP_MODE);

	register_data &= ~(ADS1115_DR);			// clear
	register_data |= ADS1115_DR_2;

	register_data |= (ADS1115_MODE);
	register_data &= ~(ADS1115_PGA);

	register_data &= ~(ADS1115_MUX);		// clear

	switch (channel) {
	case 0:
		register_data |= ADS1115_MUX_2;
		break;
	case 1:
		register_data |= (ADS1115_MUX_2 | ADS1115_MUX_0);
		break;
	case 2:
		register_data |= (ADS1115_MUX_2 | ADS1115_MUX_1);
		break;
	case 3:
		register_data |= ADS1115_MUX;
		break;
	}

	register_data |= ADS1115_OS;

	data[0] = register_data >> 8;		// msb
	data[1] = register_data & 0xFF;		// lsb

	ADS1115_WriteRegisters(ADS1115_REG_CONFIG, data, 2);

	DWT_delay_ms(10);

	ADS1115_ReadRegisters(ADS1115_REG_CONVERSION, data, 2);

	received_data = (int16_t)((data[0] << 8) | data[1]);

	if (received_data < 0) {
		received_data = 0;
	}

	return received_data;
}
