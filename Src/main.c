#include "main.h"

char usart_buffer[200];
uint16_t counter = 0;

int main() {

	RCC_Init();
	DWT_Init();
	GPIO_Init();

	// USART1_Init();
	I2C1_Init();
	// SPI1_Init();

	LCD1602_Init();
	LCD1602_Led(LCD1602_ON);

	LCD1602_CreateChar(SMILE, LCD1602_SMILE);
	LCD1602_CreateChar(CHEST, LCD1602_CHEST);
	LCD1602_CreateChar(HUMAN, LCD1602_HUMAN);
	LCD1602_CreateChar(HOUSE, LCD1602_HOUSE);
	LCD1602_CreateChar(BATTARY, LCD1602_BATTARY);
	LCD1602_CreateChar(ANTENNA, LCD1602_ANTENNA);

	LCD1602_CursorPos(0, 0);
	LCD1602_SendString("Welcome!");
	LCD1602_CursorPos(1, 0);
	LCD1602_SendChar(LCD1602_SMILE);
	LCD1602_SendChar(LCD1602_CHEST);
	LCD1602_SendChar(LCD1602_HUMAN);
	LCD1602_SendChar(LCD1602_HOUSE);

	LCD1602_CursorPos(0, 15);
	LCD1602_SendChar(LCD1602_ANTENNA);
	LCD1602_CursorPos(1, 15);
	LCD1602_SendChar(LCD1602_BATTARY);

	LCD1602_CursorPos(0, 8);
	sprintf(usart_buffer, "%.2f", 123.45f);
	LCD1602_SendString(usart_buffer);

	while (1) {

		sprintf(usart_buffer, "%d", counter);

		LCD1602_CursorPos(1, 8);
		LCD1602_SendString(usart_buffer);

		counter++;

		DWT_delay_ms(200);


//		sprintf(usart_buffer, "Estimated: %lu\r\n", estimated);
//		USART1_SendString(usart_buffer);
//		GPIO_PC13_On();
//		GPIO_PC13_Off();
//		DWT_delay_ms(200);
	}
	return 0;
}
