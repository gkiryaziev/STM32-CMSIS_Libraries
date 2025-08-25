/*
 * PCF8574T.h
 *
 * Updated at: 31.05.2025
 * Created on: 25.05.2019
 *     Author: Admin
 */

#ifndef PCF8574T_H_
#define PCF8574T_H_

#ifdef __cplusplus
extern "C" {
#endif /* __cplusplus */

#include "stm32f1xx.h"
#include "i2c.h"

#define PCF8574T_ADDRESS        0x27

void PCF8574T_Init(void);
I2C_Status_Type PCF8574T_GetStatus(void);
I2C_Status_Type PCF8574T_Read(uint8_t *data);
I2C_Status_Type PCF8574T_Write(uint8_t data);
I2C_Status_Type PCF8574T_ReadPin(uint8_t pin, uint8_t *state);
I2C_Status_Type PCF8574T_WritePin(uint8_t pin, uint8_t state);
uint8_t PCF8574T_GetPortState(void);

#ifdef __cplusplus
}
#endif /* __cplusplus */

#endif /* PCF8574T_H_ */
