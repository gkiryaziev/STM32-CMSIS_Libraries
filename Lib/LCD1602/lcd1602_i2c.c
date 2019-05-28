/*
 * lcd1602_i2c.c
 *
 *  Created on: 27.05.2019
 *      Author: Admin
 *
 *  SCL1	- PB6
 *  SDA1	- PB7
 */

#include "lcd1602_i2c.h"

volatile uint8_t LCD1602_Led_State = 0;

// -------------------------------------------
// Display init
// -------------------------------------------
void LCD1602_Init(void) {

	PCF8574T_WritePin(LCD1602_E_Pin, 0);	// E  LOW

	DWT_delay_ms(50);
	LCD1602_SendCommand_8bit(0x30);			// 8-bit mode
	LCD1602_SendCommand_8bit(0x30);			// 8-bit mode
	LCD1602_SendCommand_8bit(0x30);			// 8-bit mode
	LCD1602_SendCommand_8bit(0x20);			// 4-bit mode

	LCD1602_SendCommand(0x2C);				// 4-bit mode, 2 lines, font 5x7
	LCD1602_SendCommand(0x08);				// display off
	LCD1602_Clear();						// display clear
	LCD1602_SendCommand(0x06);				// cursor increment, no shift
	LCD1602_SendCommand(0x0C);				// display on, cursor off, blinking off
}

// -------------------------------------------
// Main send function
// -------------------------------------------
void LCD1602_Send(LCD1602_Data_Type dataType, uint8_t data) {

	uint8_t buffer = data;

	if (LCD1602_Led_State == LCD1602_ON)
		buffer |= (1 << LCD1602_LED_Pin);

	buffer |= (1 << LCD1602_E_Pin);			// E HIGH

	if (dataType == LCD1602_DATA)
		buffer |= (1 << LCD1602_RS_Pin);

	PCF8574T_Write(buffer);

	DWT_delay_us(300);

	buffer &= ~(1 << LCD1602_E_Pin);		// E LOW

	PCF8574T_Write(buffer);

	DWT_delay_us(300);
}

// -------------------------------------------
// Send command in 4-bit mode
// -------------------------------------------
void LCD1602_SendCommand(uint8_t cmd) {

	LCD1602_Send(LCD1602_COMMAND, cmd & 0xF0);
	DWT_delay_ms(1);
	LCD1602_Send(LCD1602_COMMAND, (cmd << 4) & 0xF0);
	DWT_delay_ms(1);
}

// -------------------------------------------
// Send command in 8-bit mode
// -------------------------------------------
void LCD1602_SendCommand_8bit(uint8_t cmd) {

	LCD1602_Send(LCD1602_COMMAND, cmd & 0xF0);
	DWT_delay_ms(1);
}

// -------------------------------------------
// Send char
// -------------------------------------------
void LCD1602_SendChar(char chr) {

	LCD1602_Send(LCD1602_DATA, (uint8_t)(chr & 0xF0));
	DWT_delay_ms(1);
	LCD1602_Send(LCD1602_DATA, (uint8_t)((chr << 4) & 0xF0));
	DWT_delay_ms(1);
}

// -------------------------------------------
// Send string
// -------------------------------------------
void LCD1602_SendString(char *str) {

	while (*str) {
		LCD1602_SendChar(*str++);
	}
}

// -------------------------------------------
// Clear display
// -------------------------------------------
void LCD1602_Clear(void) {

	LCD1602_SendCommand(0x01);		// display clear
	DWT_delay_ms(2);
}

// -------------------------------------------
// Set cursor position
// -------------------------------------------
void LCD1602_CursorPos(uint8_t x, uint8_t y) {

	uint8_t addr = 0;

	switch(x) {
		case 0: addr = 0x00; break;
		case 1: addr = 0x40; break;
		case 2: addr = 0x14; break;		// 2004
		case 3: addr = 0x54; break;		// 2004
	}

	addr += y;

	LCD1602_SendCommand(0x80 | addr);
}

// -------------------------------------------
// Create custom char
// -------------------------------------------
void LCD1602_CreateChar(uint8_t *chr, uint8_t addr) {

	LCD1602_SendCommand(0x40 + (addr * 8));

	for (uint8_t i = 0; i < 8; ++i) {
		LCD1602_SendChar(chr[i]);
	}
}

// -------------------------------------------
// Led on/off
// -------------------------------------------
void LCD1602_Led(LCD1602_Led_Type state) {

	LCD1602_Led_State = (uint8_t)state;
}
