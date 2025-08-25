/*
 * lcd1602_8bit.h
 *
 * Updated at: 05.06.2025
 * Created on: 16.05.2019
 *     Author: Admin
 *
 * E  - B3
 * RS - B4
 * D0 - B8
 * D1 - B9
 * D2 - B10
 * D3 - B11
 * D4 - B12
 * D5 - B13
 * D6 - B14
 * D7 - B15
 */

#ifndef LCD1602_8BIT_H_
#define LCD1602_8BIT_H_

#ifdef __cplusplus
extern "C" {
#endif /* __cplusplus */

#include "stm32f1xx.h"
#include "gpio.h"
#include "delay.h"

#define LCD1602_E_Port       GPIOB
#define LCD1602_E_Pin        3
#define LCD1602_RS_Port      GPIOB
#define LCD1602_RS_Pin       4

#define LCD1602_D0_Port      GPIOB
#define LCD1602_D0_Pin       8
#define LCD1602_D1_Port      GPIOB
#define LCD1602_D1_Pin       9
#define LCD1602_D2_Port      GPIOB
#define LCD1602_D2_Pin       10
#define LCD1602_D3_Port      GPIOB
#define LCD1602_D3_Pin       11
#define LCD1602_D4_Port      GPIOB
#define LCD1602_D4_Pin       12
#define LCD1602_D5_Port      GPIOB
#define LCD1602_D5_Pin       13
#define LCD1602_D6_Port      GPIOB
#define LCD1602_D6_Pin       14
#define LCD1602_D7_Port      GPIOB
#define LCD1602_D7_Pin       15

typedef enum {
    LCD1602_COMMAND = 0,
    LCD1602_DATA    = 1
} LCD1602_Data_Type;

void LCD1602_Init(void);
void LCD1602_Send(LCD1602_Data_Type dataType, uint8_t data);
void LCD1602_SendCommand(uint8_t cmd);
void LCD1602_SendChar(char chr);
void LCD1602_SendString(char *str);
void LCD1602_Clear(void);
void LCD1602_CursorPos(uint8_t x, uint8_t y);
void LCD1602_CreateChar(const uint8_t *pattern, uint8_t location);

#ifdef __cplusplus
}
#endif /* __cplusplus */

#endif /* LCD1602_8BIT_H_ */
