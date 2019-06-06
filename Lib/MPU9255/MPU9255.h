/*
 * MPU9255.h
 *
 *  Created on: 10.05.2019
 *      Author: Admin
 */

#ifndef MPU9255_H_
#define MPU9255_H_

#ifdef __cplusplus
extern "C" {
#endif /* __cplusplus */

#include "stm32f1xx.h"
#include "spi.h"
#include "delay.h"

#define AK8963_ADDR_I2C             	0x0C
#define MPU9255_READ_FLAG				0x80

// --------------- MPU9255 ------------------------------------
#define MPU9255_ADDR_WHOAMI 			0x73

#define MPU9255_REG_I2C_MST_CTRL		0x24	// 36 - I2C Master Control
#define MPU9255_REG_I2C_SLV0_ADDR		0x25	// 37 - I2C Slave 0 Control
#define MPU9255_REG_I2C_SLV0_REG		0x26	// 38 - I2C Slave 0 Control
#define MPU9255_REG_I2C_SLV0_CTRL		0x27	// 39 - I2C Slave 0 Control

#define MPU9255_REG_INT_PIN_CFG			0x37	// 55 - INT Pin / Bypass Enable Configuration

#define MPU9255_REG_ACCEL_XOUT_H		0x3B	// 59 - Accelerometer Measurements

#define MPU9255_REG_TEMP_OUT_H			0x41	// 65 - Temperature Measurement
#define MPU9255_REG_TEMP_OUT_L			0x42	// 66 - Temperature Measurement
#define MPU9255_REG_GYRO_XOUT_H			0x43	// 67 - Gyroscope Measurements

#define MPU9255_REG_EXT_SENS_DATA_00	0x49	// 73 - External Sensor Data

#define MPU9255_REG_I2C_SLV0_DO			0x63	// 99 - I2C Slave 0 Data Out

#define MPU9255_REG_CONFIG				0x1A	// 26 - Configuration
#define MPU9255_REG_GYRO_CONFIG			0x1B	// 27 - Gyroscope Configuration
#define MPU9255_REG_ACCEL_CONFIG		0x1C	// 28 - Accelerometer Configuration
#define MPU9255_REG_ACCEL_CONFIG_2		0x1D	// 29 - Accelerometer Configuration 2

#define MPU9255_REG_USER_CTRL			0x6A	// 106 - User Control
#define MPU9255_REG_PWR_MGMT_1			0x6B	// 107 - Power Management 1
#define MPU9255_REG_PWR_MGMT_2			0x6C	// 108 - Power Management 2

#define MPU9255_REG_WHOAMI 				0x75	// 117 - Who Am I

// --------------- Configuration bits mpu9255 -----------------
typedef enum {
	MPU9255_GYRO_FS_250DPS  = 0x00,
	MPU9255_GYRO_FS_500DPS  = 0x08,
	MPU9255_GYRO_FS_1000DPS = 0x10,
	MPU9255_GYRO_FS_2000DPS = 0x18
} GyroScaleRange_Type;

typedef enum {
	MPU9255_ACCEL_FS_2G     = 0x00,
	MPU9255_ACCEL_FS_4G     = 0x08,
	MPU9255_ACCEL_FS_8G     = 0x10,
	MPU9255_ACCEL_FS_16G    = 0x18
} AccelScaleRange_Type;

// --------------- AK8963 -------------------------------------
#define AK8963_ADDR_WHOAMI				0x48

#define AK8963_REG_WIA                 	0x00			// Device ID
#define AK8963_REG_HXL					0x03			// Measurement Data
#define AK8963_REG_HXH					0x04			// Measurement Data
#define AK8963_REG_HYL					0x05			// Measurement Data
#define AK8963_REG_HYH					0x06			// Measurement Data
#define AK8963_REG_HZL					0x07			// Measurement Data
#define AK8963_REG_HZH					0x08			// Measurement Data
#define AK8963_REG_CNTL1				0x0A			// Control 1
#define AK8963_REG_CNTL2				0x0B			// Control 2
#define AK8963_REG_ASAX					0x10			// Sensitivity Adjustment values
#define AK8963_REG_ASAY					0x11			// Sensitivity Adjustment values
#define AK8963_REG_ASAZ					0x12			// Sensitivity Adjustment values

#define AK8963_MAG_FS_4800UT			((float)0.6)	// 0.6 uT/LSB

typedef struct {
	float GyroX;
	int16_t GyroX_ADC;
	float GyroY;
	int16_t GyroY_ADC;
	float GyroZ;
	int16_t GyroZ_ADC;

	float AccelX;
	int16_t AccelX_ADC;
	float AccelY;
	int16_t AccelY_ADC;
	float AccelZ;
	int16_t AccelZ_ADC;

	float MagX;
	int16_t MagX_ADC;
	float MagY;
	int16_t MagY_ADC;
	float MagZ;
	int16_t MagZ_ADC;

	int8_t Temperature;

	GyroScaleRange_Type GyroScaleRange;
	AccelScaleRange_Type AccelScaleRange;
} MPU9255_TypeDef;


uint8_t MPU9255_WriteRegister(uint8_t address, uint8_t data);
uint8_t MPU9255_ReadRegister(uint8_t address);
void MPU9255_ReadRegisters(uint8_t *buffer, uint8_t address, uint8_t size);

void MPU9255_SetGyroScaleRange(GyroScaleRange_Type scale);
void MPU9255_SetAccelScaleRange(AccelScaleRange_Type scale);

void MPU9255_Init(MPU9255_TypeDef *mpu9255);
uint8_t MPU9255_GetStatus(void);
void MPU9255_GetTemperature(MPU9255_TypeDef *mpu9255);
void MPU9255_GetGyroscope(MPU9255_TypeDef *mpu9255);
void MPU9255_GetAccelerometer(MPU9255_TypeDef *mpu9255);

uint8_t AK8963_GetStatus(void);
uint8_t AK8963_GetCNTL1(void);
void MPU9255_CalibMagnetometer(void);
void MPU9255_GetMagnetometer(MPU9255_TypeDef *mpu9255);

#ifdef __cplusplus
}
#endif /* __cplusplus */

#endif /* MPU9255_H_ */
