/*
 * LSM303DLHC.c
 *
 *  Created on: 25.05.2019
 *      Author: Admin
 */

#include "LSM303DLHC.h"

void LSM303DLHC_Init(void) {}

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
