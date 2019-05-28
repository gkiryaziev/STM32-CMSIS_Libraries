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

#define LSM303DLHC_ACCEL_ADDRESS		0x19
#define LSM303DLHC_MAG_ADDRESS			0x1E

void LSM303DLHC_Init(void);
uint8_t LSM303DLHC_GetStatus(void);

#ifdef __cplusplus
}
#endif /* __cplusplus */

#endif /* LSM303DLHC_H_ */
