/*
 * LSM303DLHC.c
 *
 *  Created on: 25.05.2019
 *      Author: Admin
 *
 *  SCL1	- PB6
 *  SDA1	- PB7
 */

#include "LSM303DLHC.h"

uint16_t mag_gain_x_y, mag_gain_z;

uint8_t LSM303DLHC_ReadRegister(uint8_t address, uint8_t reg_address) {

	// I2C Send
	I2C1_Start();
	I2C1_SendAddress(address, I2C_TRANSMITTER);
	I2C1_SendData(reg_address);

	// I2C Receive
	I2C1_Start();
	I2C1_SendAddress(address, I2C_RECEIVER);
	I2C1_Stop();

	return I2C1_ReceiveData(I2C_NACK);
}

void LSM303DLHC_ReadRegisters(uint8_t address, uint8_t reg_address, uint8_t *data, uint8_t size) {

	uint8_t count = 0;

	// I2C Send
	I2C1_Start();
	I2C1_SendAddress(address, I2C_TRANSMITTER);
	I2C1_SendData(reg_address);

	// I2C Receive
	I2C1_Start();
	I2C1_SendAddress(address, I2C_RECEIVER);

	for (count = 0; count < size - 1; ++count ) {
		data[count] = I2C1_ReceiveData(I2C_ACK);
	}

	I2C1_Stop();
	data[count] = I2C1_ReceiveData(I2C_NACK);
}

void LSM303DLHC_WriteRegister(uint8_t address, uint8_t reg_address, uint8_t data) {

	// I2C Send
	I2C1_Start();
	I2C1_SendAddress(address, I2C_TRANSMITTER);
	I2C1_SendData(reg_address);
	I2C1_SendData(data);
	I2C1_Stop();
}

uint8_t LSM303DLHC_GetStatus(void) {

	uint8_t accel_status = 0;
	uint8_t mag_status = 0;

	// I2C Send
	I2C1_Start();
	accel_status = I2C1_SendAddress(LSM303DLHC_ACCEL_ADDRESS, I2C_TRANSMITTER);
	I2C1_Stop();

	// I2C Send
	I2C1_Start();
	mag_status = I2C1_SendAddress(LSM303DLHC_MAG_ADDRESS, I2C_TRANSMITTER);
	I2C1_Stop();

	if ((accel_status == I2C_ERROR) || (mag_status == I2C_ERROR)) return 0;

	return 1;
}

void LSM303DLHC_Init(void) {

	// Mag init
	LSM303DLHC_SetMagnetometerDataRate(LSM303DLHC_MAG_DATA_RATE_15);
	LSM303DLHC_SetMagnetometerRange(LSM303DLHC_MAG_RANGE_1_3);
	LSM303DLHC_SetMagnetometerMode(LSM303DLHC_MAG_MODE_CONTINUOUS);
}

int16_t LSM303DLHC_GetTemperature(void) {

	uint8_t data[2] = {0};

	LSM303DLHC_ReadRegisters(LSM303DLHC_MAG_ADDRESS, LSM303DLHC_TEMP_OUT_H_M, data, 2);

	return (int16_t)((data[0] << 4) | (data[1] >> 4));
}

void LSM303DLHC_GetMagnetometer(LSM303DLHC_TypeDef *lsm303dlhc) {

	uint8_t data[6] = {0};

	LSM303DLHC_ReadRegisters(LSM303DLHC_MAG_ADDRESS, LSM303DLHC_OUT_X_H_M, data, 6);

	lsm303dlhc->MagX_ADC = (int16_t)((data[0] << 8) | data[1]);
	lsm303dlhc->MagY_ADC = (int16_t)((data[2] << 8) | data[3]);
	lsm303dlhc->MagZ_ADC = (int16_t)((data[4] << 8) | data[5]);

	lsm303dlhc->MagX = (float)lsm303dlhc->MagX_ADC / (float)mag_gain_x_y;
	lsm303dlhc->MagY = (float)lsm303dlhc->MagY_ADC / (float)mag_gain_x_y;
	lsm303dlhc->MagZ = (float)lsm303dlhc->MagZ_ADC / (float)mag_gain_z;
}

void LSM303DLHC_SetMagnetometerDataRate(LSM303DLHC_Mag_DataRate_Type rate) {

	uint8_t tempval = 0;

	tempval |= (1 << 7);		// 1: temperature sensor enabled
	tempval |= (rate << 2);

	LSM303DLHC_WriteRegister(LSM303DLHC_MAG_ADDRESS, LSM303DLHC_CRA_REG_M, tempval);
}

void LSM303DLHC_SetMagnetometerRange(LSM303DLHC_Mag_Range_Type range) {

	uint8_t tempval = 0;

	tempval |= (range << 5);

	LSM303DLHC_WriteRegister(LSM303DLHC_MAG_ADDRESS, LSM303DLHC_CRB_REG_M, tempval);

	switch (range) {
	case LSM303DLHC_MAG_RANGE_1_3:
		mag_gain_x_y = 1100;
		mag_gain_z = 980;
		break;
	case LSM303DLHC_MAG_RANGE_1_9:
		mag_gain_x_y = 855;
		mag_gain_z = 760;
		break;
	case LSM303DLHC_MAG_RANGE_2_5:
		mag_gain_x_y = 670;
		mag_gain_z = 600;
		break;
	case LSM303DLHC_MAG_RANGE_4_0:
		mag_gain_x_y = 450;
		mag_gain_z = 400;
		break;
	case LSM303DLHC_MAG_RANGE_4_7:
		mag_gain_x_y = 400;
		mag_gain_z = 355;
		break;
	case LSM303DLHC_MAG_RANGE_5_6:
		mag_gain_x_y = 330;
		mag_gain_z = 295;
		break;
	case LSM303DLHC_MAG_RANGE_8_1:
		mag_gain_x_y = 230;
		mag_gain_z = 205;
		break;
	}
}

void LSM303DLHC_SetMagnetometerMode(LSM303DLHC_Mag_Mode_Type mode) {

	uint8_t tempval = 0;

	tempval |= (mode << 0);

	LSM303DLHC_WriteRegister(LSM303DLHC_MAG_ADDRESS, LSM303DLHC_MR_REG_M, tempval);
}
