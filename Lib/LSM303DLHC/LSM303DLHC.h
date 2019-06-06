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
	int16_t MagX_ADC;
	int16_t MagY_ADC;
	int16_t MagZ_ADC;
	float MagX;
	float MagY;
	float MagZ;
} LSM303DLHC_TypeDef;

typedef enum {
	LSM303DLHC_MAG_DATA_RATE_0_75  = 0x0,
	LSM303DLHC_MAG_DATA_RATE_1_5   = 0x1,
	LSM303DLHC_MAG_DATA_RATE_3_0   = 0x2,
	LSM303DLHC_MAG_DATA_RATE_7_5   = 0x3,
	LSM303DLHC_MAG_DATA_RATE_15    = 0x4,
	LSM303DLHC_MAG_DATA_RATE_30    = 0x5,
	LSM303DLHC_MAG_DATA_RATE_75    = 0x6,
	LSM303DLHC_MAG_DATA_RATE_220   = 0x7
} LSM303DLHC_Mag_DataRate_Type;

typedef enum {
	LSM303DLHC_MAG_RANGE_1_3       = 0x1,
	LSM303DLHC_MAG_RANGE_1_9       = 0x2,
	LSM303DLHC_MAG_RANGE_2_5       = 0x3,
	LSM303DLHC_MAG_RANGE_4_0       = 0x4,
	LSM303DLHC_MAG_RANGE_4_7       = 0x5,
	LSM303DLHC_MAG_RANGE_5_6       = 0x6,
	LSM303DLHC_MAG_RANGE_8_1       = 0x7
} LSM303DLHC_Mag_Range_Type;

typedef enum {
	LSM303DLHC_MAG_MODE_CONTINUOUS = 0x0,
	LSM303DLHC_MAG_MODE_SINGLE     = 0x1,
	LSM303DLHC_MAG_MODE_SLEEP      = 0x3,
} LSM303DLHC_Mag_Mode_Type;

uint8_t LSM303DLHC_ReadRegister(uint8_t address, uint8_t reg_address);
void LSM303DLHC_ReadRegisters(uint8_t address, uint8_t reg_address, uint8_t *data, uint8_t size);
void LSM303DLHC_WriteRegister(uint8_t address, uint8_t reg_address, uint8_t data);

uint8_t LSM303DLHC_GetStatus(void);
void LSM303DLHC_Init(void);
int16_t LSM303DLHC_GetTemperature(void);
void LSM303DLHC_GetMagnetometer(LSM303DLHC_TypeDef *lsm303dlhc);

void LSM303DLHC_SetMagnetometerDataRate(LSM303DLHC_Mag_DataRate_Type rate);
void LSM303DLHC_SetMagnetometerRange(LSM303DLHC_Mag_Range_Type range);
void LSM303DLHC_SetMagnetometerMode(LSM303DLHC_Mag_Mode_Type mode);

#ifdef __cplusplus
}
#endif /* __cplusplus */

#endif /* LSM303DLHC_H_ */
