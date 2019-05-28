/*
 * lcd1602_char.h
 *
 *  Created on: 28.05.2019
 *      Author: Admin
 */

#ifndef LCD1602_CHAR_H_
#define LCD1602_CHAR_H_

#ifdef __cplusplus
extern "C" {
#endif /* __cplusplus */

typedef enum {
	LCD1602_SMILE = 0x0,
	LCD1602_CHEST,
	LCD1602_HUMAN,
	LCD1602_HOUSE,
	LCD1602_BATTARY,
	LCD1602_ANTENNA
} LCD1602_CustomChar_Type;

uint8_t SMILE[8]   = { 0b00000, 0b01010, 0b11011, 0b01110, 0b00100, 0b10001, 0b01110, 0b00000 };
uint8_t CHEST[8]   = { 0b00100, 0b01110, 0b11011, 0b01110, 0b00100, 0b00100, 0b11111, 0b00100 };
uint8_t HUMAN[8]   = { 0b11111, 0b01110, 0b00100, 0b11111, 0b00100, 0b00100, 0b01010, 0b10001 };
uint8_t HOUSE[8]   = { 0b00100, 0b01010, 0b10001, 0b11111, 0b10001, 0b10101, 0b10001, 0b11111 };
uint8_t BATTARY[8] = { 0b00000, 0b01110, 0b11111, 0b10001, 0b10001, 0b11111, 0b11111, 0b11111 };
uint8_t ANTENNA[8] = { 0b00000, 0b11111, 0b10001, 0b01110, 0b00100, 0b00100, 0b00100, 0b00100 };

#ifdef __cplusplus
}
#endif /* __cplusplus */

#endif /* LCD1602_CHAR_H_ */
