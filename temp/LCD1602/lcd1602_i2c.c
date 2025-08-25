/*
 * lcd1602_i2c.c
 *
 * Updated at: 31.05.2025
 * Created on: 27.05.2019
 *     Author: Admin
 *
 * SCL1    - PB6
 * SDA1    - PB7
 */

#include "lcd1602_i2c.h"

volatile LCD1602_LedState LCD1602_Led_State = LCD1602_LED_OFF;

// -------------------------------------------
// LCD1602_Init
// -------------------------------------------
void LCD1602_Init(LCD1602_LedState led_state) {

    uint8_t pcf_state = 0;
    if (led_state == LCD1602_LED_ON) {
        pcf_state |= (1 << LCD1602_LED_Pin);
    }
    PCF8574T_Write(pcf_state);                         // RS, RW, E LOW
    LCD1602_Led_State = led_state;

    DWT_delay_ms(50);
    LCD1602_SendCommand_8bit(0x30); DWT_delay_ms(5);   // 8-bit mode
    LCD1602_SendCommand_8bit(0x30); DWT_delay_us(150); // 8-bit mode
    LCD1602_SendCommand_8bit(0x30); DWT_delay_ms(1);   // 8-bit mode
    LCD1602_SendCommand_8bit(0x20); DWT_delay_ms(1);   // 4-bit mode
    
	LCD1602_SendCommand(0x28);                         // 4-bit mode, 2 lines, 5x8 font
    LCD1602_SendCommand(0x08);                         // display off
    LCD1602_Clear();                                   // display clear
    LCD1602_SendCommand(0x06);                         // cursor increment, no shift
    LCD1602_SendCommand(0x0C);                         // display on, cursor off, blinking off
}

// -------------------------------------------
// LCD1602_Send
// -------------------------------------------
void LCD1602_Send(LCD1602_Data_Type dataType, uint8_t data) {

    uint8_t buffer = 0;

    buffer |= (data & 0xF0);

    if (LCD1602_Led_State == LCD1602_LED_ON)
        buffer |= (1 << LCD1602_LED_Pin);

    if (dataType == LCD1602_DATA)
        buffer |= (1 << LCD1602_RS_Pin);

    PCF8574T_Write(buffer | (1 << LCD1602_E_Pin));
    DWT_delay_us(1);

    PCF8574T_Write(buffer);
    DWT_delay_us(100);
}

// -------------------------------------------
// LCD1602_SendCommand (4-bit mode)
// -------------------------------------------
void LCD1602_SendCommand(uint8_t cmd) {

    LCD1602_Send(LCD1602_COMMAND, cmd & 0xF0);
    LCD1602_Send(LCD1602_COMMAND, (cmd << 4) & 0xF0);
    DWT_delay_us(100);
}

// -------------------------------------------
// LCD1602_SendCommand_8bit
// -------------------------------------------
void LCD1602_SendCommand_8bit(uint8_t cmd) {

    LCD1602_Send(LCD1602_COMMAND, cmd & 0xF0);
}

// -------------------------------------------
// LCD1602_SendChar
// -------------------------------------------
void LCD1602_SendChar(char chr) {

    LCD1602_Send(LCD1602_DATA, (uint8_t)(chr & 0xF0));
    LCD1602_Send(LCD1602_DATA, (uint8_t)((chr << 4) & 0xF0));
    DWT_delay_us(40);
}

// -------------------------------------------
// LCD1602_SendString
// -------------------------------------------
void LCD1602_SendString(char *str) {

    while (*str) {
        LCD1602_SendChar(*str++);
    }
}

// -------------------------------------------
// LCD1602_Clear
// -------------------------------------------
void LCD1602_Clear(void) {

    LCD1602_SendCommand(0x01);                   // display clear
    DWT_delay_ms(2);
}

// -------------------------------------------
// LCD1602_CursorPos
// -------------------------------------------
void LCD1602_CursorPos(uint8_t x, uint8_t y) {

    uint8_t addr = 0;

    switch(x) {
        case 0: addr = 0x00; break;
        case 1: addr = 0x40; break;
        case 2: addr = 0x14; break;              // 2004
        case 3: addr = 0x54; break;              // 2004
    }

    addr += y;

    LCD1602_SendCommand(0x80 | addr);
}

// -------------------------------------------
// LCD1602_CreateChar
// -------------------------------------------
void LCD1602_CreateChar(const uint8_t *pattern, uint8_t location) {

    LCD1602_SendCommand(0x40 + (location * 8));

    for (uint8_t i = 0; i < 8; ++i) {
        LCD1602_SendChar(pattern[i]);
    }
}

// -------------------------------------------
// LCD1602_Led (ON/OFF)
// -------------------------------------------
void LCD1602_Led(LCD1602_LedState state) {

    LCD1602_Led_State = state;

    uint8_t port_state = PCF8574T_GetPortState();

    if (LCD1602_Led_State == LCD1602_LED_ON)
        port_state |= (1 << LCD1602_LED_Pin);
    else
        port_state &= ~(1 << LCD1602_LED_Pin);

    PCF8574T_Write(port_state); 
}
