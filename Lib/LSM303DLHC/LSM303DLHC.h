/*
 * LSM303DLHC.h
 *
 *  Created on: 25.05.2019
 *      Author: Admin
 */

#ifndef LSM303DLHC_H_
#define LSM303DLHC_H_

#ifdef __cplusplus
extern "C" {
#endif /* __cplusplus */

#include "stm32f1xx.h"
#include "delay.h"
#include "gpio.h"
#include "i2c.h"

#define LSM303DLHC_MULTIPLE_FLAG		0x80

#define LSM303DLHC_ACCEL_ADDRESS		0x19
#define LSM303DLHC_MAG_ADDRESS			0x1E

// Accel
#define LSM303DLHC_CTRL_REG1_A			0x20	// Data rate, Low-power mode, Z,Y,X-axis enable.
#define LSM303DLHC_CTRL_REG2_A			0x21	// High-pass filter.
#define LSM303DLHC_CTRL_REG3_A			0x22	// Interrupts INT1.
#define LSM303DLHC_CTRL_REG4_A			0x23	// Update, Scale, Resolution, etc.
#define LSM303DLHC_CTRL_REG5_A			0x24	// Reboot memory, FIFO, etc.
#define LSM303DLHC_CTRL_REG6_A			0x25	// Interrupts PAD2
#define LSM303DLHC_REFERENCE_A			0x26
#define LSM303DLHC_STATUS_REG_A			0x27
#define LSM303DLHC_OUT_X_L_A			0x28
#define LSM303DLHC_OUT_X_H_A			0x29
#define LSM303DLHC_OUT_Y_L_A			0x2A
#define LSM303DLHC_OUT_Y_H_A			0x2B
#define LSM303DLHC_OUT_Z_L_A			0x2C
#define LSM303DLHC_OUT_Z_H_A			0x2D

// Mag
#define LSM303DLHC_CRA_REG_M			0x00
#define LSM303DLHC_CRB_REG_M			0x01
#define LSM303DLHC_MR_REG_M				0x02
#define LSM303DLHC_OUT_X_H_M			0x03
#define LSM303DLHC_OUT_X_L_M			0x04
#define LSM303DLHC_OUT_Z_H_M			0x05
#define LSM303DLHC_OUT_Z_L_M			0x06
#define LSM303DLHC_OUT_Y_H_M			0x07
#define LSM303DLHC_OUT_Y_L_M			0x08
#define LSM303DLHC_SR_REG_M				0x09
#define LSM303DLHC_IRA_REG_M			0x0A
#define LSM303DLHC_IRB_REG_M			0x0B
#define LSM303DLHC_IRC_REG_M			0x0C
#define LSM303DLHC_TEMP_OUT_H_M			0x31
#define LSM303DLHC_TEMP_OUT_L_M			0x32

typedef struct {
	int16_t AccelX_ADC;
	int16_t AccelY_ADC;
	int16_t AccelZ_ADC;
	int16_t MagX_ADC;
	int16_t MagY_ADC;
	int16_t MagZ_ADC;
	float AccelX;
	float AccelY;
	float AccelZ;
	float MagX;
	float MagY;
	float MagZ;
} LSM303DLHC_TypeDef;

typedef enum {
	LSM303DLHC_ACCEL_DATA_RATE_OFF      = 0x0,
	LSM303DLHC_ACCEL_DATA_RATE_1HZ      = 0x1,
	LSM303DLHC_ACCEL_DATA_RATE_10HZ     = 0x2,
	LSM303DLHC_ACCEL_DATA_RATE_25HZ     = 0x3,
	LSM303DLHC_ACCEL_DATA_RATE_50HZ     = 0x4,
	LSM303DLHC_ACCEL_DATA_RATE_100HZ    = 0x5,
	LSM303DLHC_ACCEL_DATA_RATE_200HZ    = 0x6,
	LSM303DLHC_ACCEL_DATA_RATE_400HZ    = 0x7,
	LSM303DLHC_ACCEL_DATA_RATE_1_620kHZ = 0x8,	// Low-power mode
	LSM303DLHC_ACCEL_DATA_RATE_1_344kHZ = 0x9	// Low-power mode (5.376 kHz)
} LSM303DLHC_Accel_DataRate_Type;

typedef enum {
	LSM303DLHC_ACCEL_SCALE_2G           = 0x0,
	LSM303DLHC_ACCEL_SCALE_4G           = 0x1,
	LSM303DLHC_ACCEL_SCALE_8G           = 0x2,
	LSM303DLHC_ACCEL_SCALE_16G          = 0x3
} LSM303DLHC_Accel_Scale_Type;


typedef enum {
	LSM303DLHC_MAG_DATA_RATE_0_75HZ     = 0x0,
	LSM303DLHC_MAG_DATA_RATE_1_5HZ      = 0x1,
	LSM303DLHC_MAG_DATA_RATE_3_0HZ      = 0x2,
	LSM303DLHC_MAG_DATA_RATE_7_5HZ      = 0x3,
	LSM303DLHC_MAG_DATA_RATE_15HZ       = 0x4,
	LSM303DLHC_MAG_DATA_RATE_30HZ       = 0x5,
	LSM303DLHC_MAG_DATA_RATE_75HZ       = 0x6,
	LSM303DLHC_MAG_DATA_RATE_220HZ      = 0x7
} LSM303DLHC_Mag_DataRate_Type;

typedef enum {
	LSM303DLHC_MAG_RANGE_1_3GAUSS       = 0x1,
	LSM303DLHC_MAG_RANGE_1_9GAUSS       = 0x2,
	LSM303DLHC_MAG_RANGE_2_5GAUSS       = 0x3,
	LSM303DLHC_MAG_RANGE_4_0GAUSS       = 0x4,
	LSM303DLHC_MAG_RANGE_4_7GAUSS       = 0x5,
	LSM303DLHC_MAG_RANGE_5_6GAUSS       = 0x6,
	LSM303DLHC_MAG_RANGE_8_1GAUSS       = 0x7
} LSM303DLHC_Mag_Range_Type;

typedef enum {
	LSM303DLHC_MAG_MODE_CONTINUOUS      = 0x0,
	LSM303DLHC_MAG_MODE_SINGLE          = 0x1,
	LSM303DLHC_MAG_MODE_SLEEP           = 0x3
} LSM303DLHC_Mag_Mode_Type;

uint8_t LSM303DLHC_ReadRegister(uint8_t address, uint8_t reg_address);
void LSM303DLHC_ReadRegisters(uint8_t address, uint8_t reg_address, uint8_t *data, uint8_t size);
void LSM303DLHC_WriteRegister(uint8_t address, uint8_t reg_address, uint8_t data);
void LSM303DLHC_WriteRegisters(uint8_t address, uint8_t reg_address, uint8_t *data, uint8_t size);
void LSM303DLHC_SetAccelerometerDataRate(LSM303DLHC_Accel_DataRate_Type rate);
void LSM303DLHC_SetAccelerometerScale(LSM303DLHC_Accel_Scale_Type scale);
void LSM303DLHC_SetMagnetometerDataRate(LSM303DLHC_Mag_DataRate_Type rate);
void LSM303DLHC_SetMagnetometerRange(LSM303DLHC_Mag_Range_Type range);
void LSM303DLHC_SetMagnetometerMode(LSM303DLHC_Mag_Mode_Type mode);
uint8_t LSM303DLHC_GetStatus(void);
void LSM303DLHC_Init(void);
int16_t LSM303DLHC_GetTemperature(void);
void LSM303DLHC_GetAccelerometer(LSM303DLHC_TypeDef *lsm303dlhc);
void LSM303DLHC_GetMagnetometer(LSM303DLHC_TypeDef *lsm303dlhc);


#ifdef __cplusplus
}
#endif /* __cplusplus */

#endif /* LSM303DLHC_H_ */
