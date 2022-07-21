![Alt text](schematic.png?raw=true "schematic")

#### Usage (2 digits example)
```c
#include "led_display.h"

HC595_Init();
LED_Display_Init();
	
for(int i = 0; i < 100; ++i)
{
	LED_Display_Write(i);
	DWT_delay_ms(500);
}
	
```