/*
 * MPU9255.c
 *
 *  Created on: 10.05.2019
 *      Author: Admin
 *
 *  NSS      - PA4 (General purpose output push-pull)	(nCS)
 *  SCK      - PA5 (Alternate function push-pull)		(SCL)
 *  MISO     - PA6 (Input floating / Input pull-up)		(AD0/SDO)
 *  MOSI     - PA7 (Alternate function push-pull)		(SDA)
 */

#include "MPU9255.h"

float gyro_scale_factor;
float accel_scale_factor;
float mag_asaX, mag_asaY, mag_asaZ;

uint8_t MPU9255_WriteRegister(uint8_t address, uint8_t data) {

	uint8_t temp_val;

	SPI1_EnableSlave();
	SPI1_Write(address);
	temp_val = SPI1_Write(data);
	SPI1_DisableSlave();

	return temp_val;
}

uint8_t MPU9255_ReadRegister(uint8_t address) {

	return MPU9255_WriteRegister(address | MPU9255_READ_FLAG, 0x00);
}

void MPU9255_ReadRegisters(uint8_t *buffer, uint8_t address, uint8_t size) {

	SPI1_EnableSlave();
	SPI1_Write(address | MPU9255_READ_FLAG);
	for (uint8_t i = 0; i < size; ++i) {
		buffer[i] = SPI1_Write(0x00);
	}
	SPI1_DisableSlave();
}

void MPU9255_SetGyroScaleRange(GyroScaleRange_Type scale) {

	MPU9255_WriteRegister(MPU9255_REG_GYRO_CONFIG, scale);

	switch (scale) {
		case MPU9255_GYRO_FS_250DPS:
			gyro_scale_factor = 131;
			break;
		case MPU9255_GYRO_FS_500DPS:
			gyro_scale_factor = 65.5;
			break;
		case MPU9255_GYRO_FS_1000DPS:
			gyro_scale_factor = 32.8;
			break;
		case MPU9255_GYRO_FS_2000DPS:
			gyro_scale_factor = 16.4;
			break;
	}
}

void MPU9255_SetAccelScaleRange(AccelScaleRange_Type scale) {

	MPU9255_WriteRegister(MPU9255_REG_ACCEL_CONFIG, scale);

	switch (scale) {
		case MPU9255_ACCEL_FS_2G:
			accel_scale_factor = 16384;
			break;
		case MPU9255_ACCEL_FS_4G:
			accel_scale_factor = 8192;
			break;
		case MPU9255_ACCEL_FS_8G:
			accel_scale_factor = 4096;
			break;
		case MPU9255_ACCEL_FS_16G:
			accel_scale_factor = 2048;
			break;
	}
}

void MPU9255_Init(MPU9255_TypeDef *mpu9255) {

	// MPU9255
	MPU9255_WriteRegister(MPU9255_REG_PWR_MGMT_1, 0x80);					// 1: Reset the internal registers and restores the default settings.

	DWT_delay_ms(50);

	MPU9255_WriteRegister(MPU9255_REG_PWR_MGMT_1, 0x01);					// 1: Auto selects the best available clock source - PLL if ready.
	MPU9255_WriteRegister(MPU9255_REG_PWR_MGMT_2, 0x00);					// 0: XYZ of accelerometer & gyro is on.
	MPU9255_WriteRegister(MPU9255_REG_CONFIG, 0x01);						// Use DLPF set gyroscope bandwidth 184Hz, temperature bandwidth 188Hz.
	MPU9255_SetGyroScaleRange(MPU9255_GYRO_FS_250DPS);						// 00: +250dps
	MPU9255_SetAccelScaleRange(MPU9255_ACCEL_FS_2G);						// 00: +/-2g
	MPU9255_WriteRegister(MPU9255_REG_ACCEL_CONFIG_2, 0x01);				// Set accelerometer data rates, enable accelerometer LPF, bandwidth 184Hz.
	MPU9255_WriteRegister(MPU9255_REG_INT_PIN_CFG, 0x12);					// 1: BYPASS_EN, 1: INT_ANYRD_2CLEAR

	// I2C Master for AK8963
	MPU9255_WriteRegister(MPU9255_REG_USER_CTRL, 0x30);						// 1: Reset I2C Slave module and put the serial interface in SPI mode only.
																			// 1: Enable the I2C Master I/F module.
	MPU9255_WriteRegister(MPU9255_REG_I2C_MST_CTRL, 0x0D);					// 1101: 400kHz

	MPU9255_WriteRegister(MPU9255_REG_I2C_SLV0_ADDR, AK8963_ADDR_I2C);		// Physical address of I2C slave 0. Transfer is a write.

	MPU9255_WriteRegister(MPU9255_REG_I2C_SLV0_REG, AK8963_REG_CNTL2);		// I2C slave 0 register address from where to begin data transfer.
	MPU9255_WriteRegister(MPU9255_REG_I2C_SLV0_DO, 0x01);					// 1: Reset AK8963.
	MPU9255_WriteRegister(MPU9255_REG_I2C_SLV0_CTRL, 0x81);					// Enable I2C. 1 byte write.

	DWT_delay_ms(50);

	MPU9255_WriteRegister(MPU9255_REG_I2C_SLV0_REG, AK8963_REG_CNTL1);		// I2C slave 0 register address from where to begin data transfer.
	MPU9255_WriteRegister(MPU9255_REG_I2C_SLV0_DO, 0x12);					// 1: 16-bit output. 0010: Continuous measurement mode 1 (8Hz).
	MPU9255_WriteRegister(MPU9255_REG_I2C_SLV0_CTRL, 0x81);					// Enable I2C. 1 byte write.

	DWT_delay_ms(50);

	MPU9255_SetGyroScaleRange(mpu9255->GyroScaleRange);
	MPU9255_SetAccelScaleRange(mpu9255->AccelScaleRange);
}

uint8_t MPU9255_GetStatus(void) {

	uint8_t data = MPU9255_ReadRegister(MPU9255_REG_WHOAMI);

	if (data == MPU9255_ADDR_WHOAMI) {
		return 1;
	}

	return 0;
}

void MPU9255_GetTemperature(MPU9255_TypeDef *mpu9255) {

	uint8_t buffer[2] = {0};
	int16_t temp_val = 0;

	MPU9255_ReadRegisters(buffer, MPU9255_REG_TEMP_OUT_H, 2);

	temp_val = (int16_t)buffer[0] << 8 | buffer[1];

	mpu9255->Temperature = (((float)temp_val - 21.0) / 333.87) + 21.0;
}

void MPU9255_GetGyroscope(MPU9255_TypeDef *mpu9255) {

	uint8_t buffer[6] = {0};

	MPU9255_ReadRegisters(buffer, MPU9255_REG_GYRO_XOUT_H, 6);

	mpu9255->GyroX_ADC = ((int16_t)buffer[0] << 8) | buffer[1];
	mpu9255->GyroX = (float)mpu9255->GyroX_ADC / gyro_scale_factor;

	mpu9255->GyroY_ADC = ((int16_t)buffer[2] << 8) | buffer[3];
	mpu9255->GyroY = (float)mpu9255->GyroY_ADC / gyro_scale_factor;

	mpu9255->GyroZ_ADC = ((int16_t)buffer[4] << 8) | buffer[5];
	mpu9255->GyroZ = (float)mpu9255->GyroZ_ADC / gyro_scale_factor;
}

void MPU9255_GetAccelerometer(MPU9255_TypeDef *mpu9255) {

	uint8_t buffer[6] = {0};

	MPU9255_ReadRegisters(buffer, MPU9255_REG_ACCEL_XOUT_H, 6);

	mpu9255->AccelX_ADC = ((int16_t)buffer[0] << 8) | buffer[1];
	mpu9255->AccelX = (float)mpu9255->AccelX_ADC / accel_scale_factor;

	mpu9255->AccelY_ADC = ((int16_t)buffer[2] << 8) | buffer[3];
	mpu9255->AccelY = (float)mpu9255->AccelY_ADC / accel_scale_factor;

	mpu9255->AccelZ_ADC = ((int16_t)buffer[4] << 8) | buffer[5];
	mpu9255->AccelZ = (float)mpu9255->AccelZ_ADC / accel_scale_factor;
}

uint8_t AK8963_GetStatus(void) {

	MPU9255_WriteRegister(MPU9255_REG_I2C_SLV0_ADDR, AK8963_ADDR_I2C | MPU9255_READ_FLAG);	// Physical address of I2C slave 0. Transfer is a read.
	MPU9255_WriteRegister(MPU9255_REG_I2C_SLV0_REG, AK8963_REG_WIA);						// I2C slave 0 register address from where to begin data transfer
	MPU9255_WriteRegister(MPU9255_REG_I2C_SLV0_CTRL, 0x81);									// Enable I2C. 1 byte read.

	DWT_delay_ms(10);

	uint8_t data = MPU9255_ReadRegister(MPU9255_REG_EXT_SENS_DATA_00);						// Read I2C

	if (data == AK8963_ADDR_WHOAMI) {
		return 1;
	}

	return 0;
}

uint8_t AK8963_GetCNTL1(void) {

	MPU9255_WriteRegister(MPU9255_REG_I2C_SLV0_ADDR, AK8963_ADDR_I2C | MPU9255_READ_FLAG);	// Physical address of I2C slave 0. Transfer is a read.
	MPU9255_WriteRegister(MPU9255_REG_I2C_SLV0_REG, AK8963_REG_CNTL1);						// I2C slave 0 register address from where to begin data transfer
	MPU9255_WriteRegister(MPU9255_REG_I2C_SLV0_CTRL, 0x81);									// Enable I2C. 1 byte read.

	DWT_delay_ms(10);

	return MPU9255_ReadRegister(MPU9255_REG_EXT_SENS_DATA_00);
}

void MPU9255_CalibMagnetometer(void) {

	uint8_t buffer[3] = {0};

	MPU9255_WriteRegister(MPU9255_REG_I2C_SLV0_ADDR, AK8963_ADDR_I2C | MPU9255_READ_FLAG);	// Physical address of I2C slave 0. Transfer is a read.
	MPU9255_WriteRegister(MPU9255_REG_I2C_SLV0_REG, AK8963_REG_ASAX);						// I2C slave 0 register address from where to begin data transfer
	MPU9255_WriteRegister(MPU9255_REG_I2C_SLV0_CTRL, 0x83);									// Enable I2C. 3 byte read.

	DWT_delay_ms(10);

	MPU9255_ReadRegisters(buffer, MPU9255_REG_EXT_SENS_DATA_00, 3);

	mag_asaX = (((float)buffer[0] - 128) / 256) + 1;
	mag_asaY = (((float)buffer[1] - 128) / 256) + 1;
	mag_asaZ = (((float)buffer[2] - 128) / 256) + 1;
}

void MPU9255_GetMagnetometer(MPU9255_TypeDef *mpu9255) {

	uint8_t buffer[7] = {0};

	MPU9255_WriteRegister(MPU9255_REG_I2C_SLV0_ADDR, AK8963_ADDR_I2C | MPU9255_READ_FLAG);	// Physical address of I2C slave 0. Transfer is a read.
	MPU9255_WriteRegister(MPU9255_REG_I2C_SLV0_REG, AK8963_REG_HXL);						// I2C slave 0 register address from where to begin data transfer
	MPU9255_WriteRegister(MPU9255_REG_I2C_SLV0_CTRL, 0x87);									// Enable I2C. 7 byte read.

	DWT_delay_ms(10);

	MPU9255_ReadRegisters(buffer, MPU9255_REG_EXT_SENS_DATA_00, 7);	// Read of ST2 register 0x09 the AK8963A will unlatch the data registers for the next measurement.

	mpu9255->MagX_ADC = ((int16_t)buffer[1] << 8) | buffer[0];
	mpu9255->MagX = (float)mpu9255->MagX_ADC * mag_asaX * AK8963_MAG_FS_4800UT;
	mpu9255->MagY_ADC = ((int16_t)buffer[3] << 8) | buffer[2];
	mpu9255->MagY = (float)mpu9255->MagY_ADC * mag_asaY * AK8963_MAG_FS_4800UT;
	mpu9255->MagZ_ADC = ((int16_t)buffer[5] << 8) | buffer[4];
	mpu9255->MagZ = (float)mpu9255->MagZ_ADC * mag_asaZ * AK8963_MAG_FS_4800UT;
}






