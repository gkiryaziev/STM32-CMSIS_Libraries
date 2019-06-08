/*
 * LSM303DLHC.c
 *
 *  Created on: 25.05.2019
 *      Author: Admin
 *
 *  Accelerometer: one 12-bit reading (left-justified)  per axis
 *  Magnetometer : one 12-bit reading (right-justified) per axis
 *
 *  - The accelerometer and magnetometer both output readings in a 16-bit format
 *  (obtained by combining the values in two 8-bit registers for each axis),
 *  but they contain a maximum of 12 bits of precision. For the accelerometer,
 *  at least the lowest 4 bits of the output values are always 0, and for the
 *  magnetometer, the highest 4 bits of the output values are always 0.
 *  - The accelerometer gives low-resolution 10-bit readings by default
 *  (the lowest 6 bits of the output are always 0). To get the full 12-bit
 *  resolution, you must set the HR (high resolution) bit in the CTRL_REG4_A register.
 *
 *  SCL1	- PB6
 *  SDA1	- PB7
 */

#include "LSM303DLHC.h"

uint16_t mag_gain_x_y, mag_gain_z;
float accel_sensitivity;

// ---------------------------------------------
// LSM303DLHC_ReadRegister
// ---------------------------------------------
uint8_t LSM303DLHC_ReadRegister(uint8_t address, uint8_t reg_address) {

	uint8_t data[1] = {0};

	LSM303DLHC_ReadRegisters(address, reg_address, data, 1);

	return data[0];
}

// ---------------------------------------------
// LSM303DLHC_ReadRegisters
// ---------------------------------------------
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

// ---------------------------------------------
// LSM303DLHC_WriteRegister
// ---------------------------------------------
void LSM303DLHC_WriteRegister(uint8_t address, uint8_t reg_address, uint8_t data) {

	uint8_t tempdata[1] = { data };

	LSM303DLHC_WriteRegisters(address, reg_address, tempdata, 1);
}

// ---------------------------------------------
// LSM303DLHC_WriteRegisters
// ---------------------------------------------
void LSM303DLHC_WriteRegisters(uint8_t address, uint8_t reg_address, uint8_t *data, uint8_t size) {

	// I2C Send
	I2C1_Start();
	I2C1_SendAddress(address, I2C_TRANSMITTER);
	I2C1_SendData(reg_address);

	for (int8_t i = 0; i < size; ++i) {
		I2C1_SendData(data[i]);
	}

	I2C1_Stop();
}

// ---------------------------------------------
// SetAccelerometerDataRate
// ---------------------------------------------
void LSM303DLHC_SetAccelerometerDataRate(LSM303DLHC_Accel_DataRate_Type rate) {

	uint8_t tempval = 0;

	tempval |= (rate << 4);		// Data rate selection.
//	tempval |= (1 << 3);		// 1: low-power mode
	tempval |= (1 << 2);		// 1: Z-axis enabled
	tempval |= (1 << 1);		// 1: Y-axis enabled
	tempval |= (1 << 0);		// 1: X-axis enabled

	LSM303DLHC_WriteRegister(LSM303DLHC_ACCEL_ADDRESS, LSM303DLHC_CTRL_REG1_A, tempval);
}

// ---------------------------------------------
// LSM303DLHC_SetAccelerometerScale
// ---------------------------------------------
void LSM303DLHC_SetAccelerometerScale(LSM303DLHC_Accel_Scale_Type scale) {

	uint8_t tempval = 0;

//	tempval |= (1 << 7);		// 0: continuous update, 1: output registers not updated until MSB and LSB have been read
//	tempval |= (1 << 6);		// 0: data LSB @ lower address, 1: data MSB @ lower address
	tempval |= (scale << 4);	// Full-scale selection.
	tempval |= (1 << 3);		// 0: high-resolution disable (10-bit), 1: high-resolution enable (12-bit)
//	tempval |= (1 << 0);		// 0: 4-wire interface, 1: 3-wire interface

	LSM303DLHC_WriteRegister(LSM303DLHC_ACCEL_ADDRESS, LSM303DLHC_CTRL_REG4_A, tempval);

	switch (scale) {
	case LSM303DLHC_ACCEL_SCALE_2G:
		accel_sensitivity = 1 / 1000.0;		// mG
		break;
	case LSM303DLHC_ACCEL_SCALE_4G:
		accel_sensitivity = 2 / 1000.0;		// mG;
		break;
	case LSM303DLHC_ACCEL_SCALE_8G:
		accel_sensitivity = 4 / 1000.0;		// mG;
		break;
	case LSM303DLHC_ACCEL_SCALE_16G:
		accel_sensitivity = 12 / 1000.0;	// mG;
		break;
	}
}

// ---------------------------------------------
// SetMagnetometerDataRate
// ---------------------------------------------
void LSM303DLHC_SetMagnetometerDataRate(LSM303DLHC_Mag_DataRate_Type rate) {

	uint8_t tempval = 0;

	tempval |= (1 << 7);		// 1: temperature sensor enabled
	tempval |= (rate << 2);

	LSM303DLHC_WriteRegister(LSM303DLHC_MAG_ADDRESS, LSM303DLHC_CRA_REG_M, tempval);
}

// ---------------------------------------------
// LSM303DLHC_SetMagnetometerRange
// ---------------------------------------------
void LSM303DLHC_SetMagnetometerRange(LSM303DLHC_Mag_Range_Type range) {

	uint8_t tempval = 0;

	tempval |= (range << 5);

	LSM303DLHC_WriteRegister(LSM303DLHC_MAG_ADDRESS, LSM303DLHC_CRB_REG_M, tempval);

	switch (range) {
	case LSM303DLHC_MAG_RANGE_1_3GAUSS:
		mag_gain_x_y = 1100;
		mag_gain_z = 980;
		break;
	case LSM303DLHC_MAG_RANGE_1_9GAUSS:
		mag_gain_x_y = 855;
		mag_gain_z = 760;
		break;
	case LSM303DLHC_MAG_RANGE_2_5GAUSS:
		mag_gain_x_y = 670;
		mag_gain_z = 600;
		break;
	case LSM303DLHC_MAG_RANGE_4_0GAUSS:
		mag_gain_x_y = 450;
		mag_gain_z = 400;
		break;
	case LSM303DLHC_MAG_RANGE_4_7GAUSS:
		mag_gain_x_y = 400;
		mag_gain_z = 355;
		break;
	case LSM303DLHC_MAG_RANGE_5_6GAUSS:
		mag_gain_x_y = 330;
		mag_gain_z = 295;
		break;
	case LSM303DLHC_MAG_RANGE_8_1GAUSS:
		mag_gain_x_y = 230;
		mag_gain_z = 205;
		break;
	}
}

// ---------------------------------------------
// LSM303DLHC_SetMagnetometerMode
// ---------------------------------------------
void LSM303DLHC_SetMagnetometerMode(LSM303DLHC_Mag_Mode_Type mode) {

	uint8_t tempval = 0;

	tempval |= (mode << 0);

	LSM303DLHC_WriteRegister(LSM303DLHC_MAG_ADDRESS, LSM303DLHC_MR_REG_M, tempval);
}

// ---------------------------------------------
// LSM303DLHC_GetStatus
// ---------------------------------------------
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

// ---------------------------------------------
// LSM303DLHC_Init
// ---------------------------------------------
void LSM303DLHC_Init(void) {

	// Accel init
	LSM303DLHC_SetAccelerometerDataRate(LSM303DLHC_ACCEL_DATA_RATE_10HZ);
	LSM303DLHC_SetAccelerometerScale(LSM303DLHC_ACCEL_SCALE_2G);
	// Mag init
	LSM303DLHC_SetMagnetometerDataRate(LSM303DLHC_MAG_DATA_RATE_15HZ);
	LSM303DLHC_SetMagnetometerRange(LSM303DLHC_MAG_RANGE_1_3GAUSS);
	LSM303DLHC_SetMagnetometerMode(LSM303DLHC_MAG_MODE_CONTINUOUS);
}

// ---------------------------------------------
// LSM303DLHC_GetTemperature
// ---------------------------------------------
int16_t LSM303DLHC_GetTemperature(void) {

	uint8_t data[2] = {0};

	LSM303DLHC_ReadRegisters(LSM303DLHC_MAG_ADDRESS, LSM303DLHC_TEMP_OUT_H_M, data, 2);

	// 12-bit ADC. 20 degrees offset picked up empirically.
	return (((int16_t)((data[0] << 8) | data[1]) >> 4) / 8) + 20;
}

// ---------------------------------------------
// LSM303DLHC_GetAccelerometer
// ---------------------------------------------
void LSM303DLHC_GetAccelerometer(LSM303DLHC_TypeDef *lsm303dlhc) {

	uint8_t data[6] = {0};

	LSM303DLHC_ReadRegisters(LSM303DLHC_ACCEL_ADDRESS, LSM303DLHC_OUT_X_L_A | LSM303DLHC_MULTIPLE_FLAG, data, 6);

	// LSB first, shift values to 6 bits for 10-bit resolution and to 4 bits for 12-bit resolution.
	lsm303dlhc->AccelX_ADC = (int16_t)((data[1] << 8) | data[0]) >> 4;
	lsm303dlhc->AccelY_ADC = (int16_t)((data[3] << 8) | data[2]) >> 4;
	lsm303dlhc->AccelZ_ADC = (int16_t)((data[5] << 8) | data[4]) >> 4;

	lsm303dlhc->AccelX = (float)lsm303dlhc->AccelX_ADC * accel_sensitivity;
	lsm303dlhc->AccelY = (float)lsm303dlhc->AccelY_ADC * accel_sensitivity;
	lsm303dlhc->AccelZ = (float)lsm303dlhc->AccelZ_ADC * accel_sensitivity;
}

// ---------------------------------------------
// LSM303DLHC_GetMagnetometer
// ---------------------------------------------
void LSM303DLHC_GetMagnetometer(LSM303DLHC_TypeDef *lsm303dlhc) {

	uint8_t data[6] = {0};

	LSM303DLHC_ReadRegisters(LSM303DLHC_MAG_ADDRESS, LSM303DLHC_OUT_X_H_M, data, 6);

	// MSB first (12-bit ADC)
	lsm303dlhc->MagX_ADC = (int16_t)((data[0] << 8) | data[1]);
	lsm303dlhc->MagY_ADC = (int16_t)((data[2] << 8) | data[3]);
	lsm303dlhc->MagZ_ADC = (int16_t)((data[4] << 8) | data[5]);

	lsm303dlhc->MagX = (float)lsm303dlhc->MagX_ADC / (float)mag_gain_x_y;
	lsm303dlhc->MagY = (float)lsm303dlhc->MagY_ADC / (float)mag_gain_x_y;
	lsm303dlhc->MagZ = (float)lsm303dlhc->MagZ_ADC / (float)mag_gain_z;
}
