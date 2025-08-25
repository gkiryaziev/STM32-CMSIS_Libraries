
#include "main.h"

Delay_TypeDef pc13 = {0};
Delay_TypeDef usart1 = {0};
Delay_TypeDef lcd1 = {0};
uint8_t pc13_state = 1;


char received_line_buffer[USART1_BUFFER_SIZE];

uint16_t counter1 = 0;
uint16_t counter2 = 10;
uint16_t counter3 = 20;
uint16_t counter4 = 30;

int main() {

    RCC_Init();
    DWT_Init();
    GPIO_Init();
    USART1_Init();
    
    I2C1_Init(I2C_STANDARD);
    LCD1602_Init(LCD1602_LED_ON);

    LCD1602_Clear();
    LCD1602_CursorPos(0, 0);
    LCD1602_SendString("Hello, World!");

    LCD1602_CursorPos(1, 0);
    LCD1602_SendString("STM32 LCD I2C");

    DWT_delay_ms(2000);

    LCD1602_Led(LCD1602_LED_OFF);
    DWT_delay_ms(1000);
    LCD1602_Led(LCD1602_LED_ON);
    DWT_delay_ms(1000);

    LCD1602_Clear();
    LCD1602_CursorPos(0, 0);
    LCD1602_SendString("Custom Char:");
    LCD1602_CreateChar(lcd1602CustomChars[LCD1602_BATTARY], LCD1602_BATTARY);
    LCD1602_CursorPos(1, 0);
    LCD1602_SendChar(LCD1602_BATTARY);

    DWT_delay_ms(2000);


    for (;;) {

        // non blocking delay, PC13
        if (DWT_nb_timeout(&pc13)) {
            GPIO_WritePin(GPIOC, 13, pc13_state);
            pc13_state = !pc13_state;
            DWT_nb_delay_ms(&pc13, 300);
        }

        // USART1
        if (DWT_nb_timeout(&usart1)) {
            uint16_t bytes_read = USART1_ReadString(received_line_buffer);
            if (bytes_read > 0) {
                USART1_SendString("Received: ");
                USART1_SendString(received_line_buffer);
                USART1_SendString("\n");
                if (strcmp(received_line_buffer, "0") == 0) counter1 = 0;
            }
            DWT_nb_delay_ms(&usart1, 50);
        }

        // LCD1
        if (DWT_nb_timeout(&lcd1)) {
            char lineBuffer[24];

            sprintf(lineBuffer, "%-7d%-7d", counter1, counter2);
            LCD1602_CursorPos(0, 0);
            LCD1602_SendString(lineBuffer);

            sprintf(lineBuffer, "%-7d%-7d", counter3, counter4);
            LCD1602_CursorPos(1, 0);
            LCD1602_SendString(lineBuffer);

            counter1++;
            counter2++;
            counter3++;
            counter4++;
            
            DWT_nb_delay_ms(&lcd1, 1000);
        }
    }

    return 0;
}
