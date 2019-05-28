/*
 * lcd1602_4bit.c
 *
 *  Created on: 16.05.2019
 *      Author: Admin
 *
 *  E  - B3
 *  RS - B4
 *  D4 - B12
 *  D5 - B13
 *  D6 - B14
 *  D7 - B15
 */

#include "lcd1602_4bit.h"

// -------------------------------------------
// Display init
// -------------------------------------------
void LCD1602_Init(void) {

	GPIO_Enable(GPIOB);

	GPIO_SetMode_Output_50MHz_PP(LCD1602_E_Port, LCD1602_E_Pin);	// E
	GPIO_SetMode_Output_50MHz_PP(LCD1602_RS_Port, LCD1602_RS_Pin);	// RS
	GPIO_SetMode_Output_50MHz_PP(LCD1602_D4_Port, LCD1602_D4_Pin);	// D4
	GPIO_SetMode_Output_50MHz_PP(LCD1602_D5_Port, LCD1602_D5_Pin);	// D5
	GPIO_SetMode_Output_50MHz_PP(LCD1602_D6_Port, LCD1602_D6_Pin);	// D6
	GPIO_SetMode_Output_50MHz_PP(LCD1602_D7_Port, LCD1602_D7_Pin);	// D7

	GPIO_WritePin(LCD1602_E_Port, LCD1602_E_Pin, 0);				// E  LOW
	GPIO_WritePin(LCD1602_RS_Port, LCD1602_RS_Pin, 0);				// RS LOW

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

	GPIO_WritePin(LCD1602_E_Port, LCD1602_E_Pin, 0);				// E LOW
	GPIO_WritePin(LCD1602_RS_Port, LCD1602_RS_Pin, dataType);		// Set RS command or data

	GPIO_WritePin(LCD1602_D4_Port, LCD1602_D4_Pin, data & 0x10);	// Check and set 4 bit
	GPIO_WritePin(LCD1602_D5_Port, LCD1602_D5_Pin, data & 0x20);	// Check and set 5 bit
	GPIO_WritePin(LCD1602_D6_Port, LCD1602_D6_Pin, data & 0x40);	// Check and set 6 bit
	GPIO_WritePin(LCD1602_D7_Port, LCD1602_D7_Pin, data & 0x80);	// Check and set 7 bit

	// Strob
	GPIO_WritePin(LCD1602_E_Port, LCD1602_E_Pin, 1);
	DWT_delay_us(500);
	GPIO_WritePin(LCD1602_E_Port, LCD1602_E_Pin, 0);
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
