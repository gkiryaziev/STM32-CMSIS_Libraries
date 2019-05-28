#include "ADS1115.h"

uint8_t ADS1115_GetStatus() {

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

	uint16_t register_data;
	int16_t received_data = 0;
	uint8_t register_address = 0, msb = 0, lsb = 0;

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

	register_address = ADS1115_CONFIG_REGISTER;

	msb = (register_data >> 8);
	lsb = (register_data & 0xFF);

	// I2C Send
	I2C1_Start();
	I2C1_SendAddress(ADS1115_ADDRESS, I2C_TRANSMITTER);
	I2C1_SendData(register_address);
	I2C1_SendData(msb);
	I2C1_SendData(lsb);
	I2C1_Stop();

	register_address = ADS1115_CONVERSION_REGISTER;

	// I2C Send
	I2C1_Start();
	I2C1_SendAddress(ADS1115_ADDRESS, I2C_TRANSMITTER);
	I2C1_SendData(register_address);
	I2C1_Stop();

	DWT_delay_ms(10);

	// I2C Receive
	I2C1_Start();
	I2C1_SendAddress(ADS1115_ADDRESS, I2C_RECEIVER);
	msb = I2C1_ReceiveData(I2C_ACK);
	I2C1_Stop();
	lsb = I2C1_ReceiveData(I2C_NACK);

	received_data = (msb << 8 | lsb);

	if (received_data < 0) {
		received_data = 0;
	}

	return received_data;
}
