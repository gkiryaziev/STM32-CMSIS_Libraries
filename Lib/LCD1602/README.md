![Alt text](lcd1602_pcf8574t.png?raw=true "lcd1602_pcf8574t")

#### Usage
```c
#include "lcd1602_char.h"
#include "lcd1602_i2c.h"

char lcd_buffer[50];

LCD1602_Init();
LCD1602_Led(LCD1602_ON);

LCD1602_CreateChar(SMILE, LCD1602_SMILE);

LCD1602_CursorPos(0, 0);
LCD1602_SendString("Welcome!");
LCD1602_CursorPos(1, 0);
LCD1602_SendChar(LCD1602_SMILE);

LCD1602_CursorPos(0, 8);
sprintf(lcd_buffer, "%.2f", 123.45f);
LCD1602_SendString(lcd_buffer);
```