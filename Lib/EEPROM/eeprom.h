/*
 * eeprom.h
 *
 *  Created on: 23 мая 2019 г.
 *      Author: Admin
 */

#ifndef EEPROM_H_
#define EEPROM_H_

#ifdef __cplusplus
extern "C" {
#endif /* __cplusplus */

#include "stm32f1xx.h"
#include "delay.h"
#include "gpio.h"
#include "i2c.h"

#define EEPROM_ADDRESS		0x50

uint8_t EEPROM_GetStatus(void);
void EEPROM_WriteByte(uint16_t address, uint8_t data);
void EEPROM_WritePage(uint16_t address, uint8_t *data, uint8_t size);
uint8_t EEPROM_ReadByte(uint16_t address);
void EEPROM_ReadPage(uint16_t address, uint8_t *data, uint8_t size);


#ifdef __cplusplus
}
#endif /* __cplusplus */

#endif /* EEPROM_H_ */
