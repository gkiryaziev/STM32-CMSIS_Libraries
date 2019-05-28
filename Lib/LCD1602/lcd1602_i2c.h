/*
 * lcd1602_i2c.h
 *
 *  Created on: 27.05.2019
 *      Author: Admin
 *
 *  SCL1	- PB6
 *  SDA1	- PB7
 */

#ifndef LCD1602_I2C_H_
#define LCD1602_I2C_H_

#ifdef __cplusplus
extern "C" {
#endif /* __cplusplus */

#include "stm32f1xx.h"
#include "gpio.h"
#include "delay.h"
#include "PCF8574T.h"		// <-- Needs for LCD1602 I2C

// PCF8574T pinout
#define LCD1602_RS_Pin		0
#define LCD1602_RW_Pin		1
#define LCD1602_E_Pin		2
#define LCD1602_LED_Pin		3
#define LCD1602_D4_Pin		4
#define LCD1602_D5_Pin		5
#define LCD1602_D6_Pin		6
#define LCD1602_D7_Pin		7

typedef enum {
	LCD1602_COMMAND = 0,
	LCD1602_DATA	= 1
} LCD1602_Data_Type;

typedef enum {
	LCD1602_OFF = 0,
	LCD1602_ON	= 1
} LCD1602_Led_Type;

void LCD1602_Init(void);
void LCD1602_Send(LCD1602_Data_Type dataType, uint8_t data);
void LCD1602_SendCommand(uint8_t cmd);
void LCD1602_SendCommand_8bit(uint8_t cmd);
void LCD1602_SendChar(char chr);
void LCD1602_SendString(char *str);
void LCD1602_Clear(void);
void LCD1602_CursorPos(uint8_t x, uint8_t y);
void LCD1602_CreateChar(uint8_t *chr, uint8_t addr);
void LCD1602_Led(LCD1602_Led_Type state);

#ifdef __cplusplus
}
#endif /* __cplusplus */

#endif /* LCD1602_I2C_H_ */
