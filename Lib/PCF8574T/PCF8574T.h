/*
 * PCF8574T.h
 *
 *  Created on: 25.05.2019
 *      Author: Admin
 */

#ifndef PCF8574T_H_
#define PCF8574T_H_

#ifdef __cplusplus
extern "C" {
#endif /* __cplusplus */

#include "stm32f1xx.h"
#include "delay.h"
#include "gpio.h"
#include "i2c.h"

#define PCF8574T_ADDRESS		0x27

void PCF8574T_Init(void);
uint8_t PCF8574T_GetStatus(void);
uint8_t PCF8574T_Read();
void PCF8574T_Write(uint8_t data);
uint8_t PCF8574T_ReadPin(uint8_t pin);
void PCF8574T_WritePin(uint8_t pin, uint8_t state);

#ifdef __cplusplus
}
#endif /* __cplusplus */

#endif /* PCF8574T_H_ */
