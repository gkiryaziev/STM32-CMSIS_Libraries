#include "main.h"

#define LED1 12
#define LED2 14

uint8_t state1, state2 = 0;
Delay_TypeDef d1, d2, usart1_d1, dht11_d1;
char buffer[USART1_BUFFER_SIZE] = { 0 }; // Set buffer size in usart.h file

char dht_tx_buffer[100] = { 0 };
DHT11_TypeDef dht11;

int main() {

	RCC_Init();
	DWT_Init();
	GPIO_Init();
	USART1_Init();

	// for two LEDs
	GPIO_Enable(GPIOB);
	GPIO_SetMode_Output_2MHz_PP(GPIOB, LED1);
	GPIO_SetMode_Output_10MHz_PP(GPIOB, LED2);

	for (;;) {

		// non blocking delay, LED1
		if (DWT_nb_timeout(&d1)) {
			GPIO_WritePin(GPIOB, LED1, state1); state1 = !state1;
			DWT_nb_delay_ms(&d1, 300);
		}

		// non blocking delay, LED2
		if (DWT_nb_timeout(&d2)) {
			GPIO_WritePin(GPIOB, LED2, state2); state2 = !state2;
			DWT_nb_delay_ms(&d2, 750);
		}

		// non blocking delay, USART1
		if (DWT_nb_timeout(&usart1_d1)) {
//			if (USART1_ReadString(buffer)) {
//				USART1_SendString(buffer); // If new data received, send data back
//			}

			char *buff = USART1_ReadStringExp();
			if (buff != 0) {

				if (buff[0] == 'l') {
					if (buff[1] == '0') { GPIO_PC13_Off(); USART1_SendString("off\n"); }
					if (buff[1] == '1') { GPIO_PC13_On();  USART1_SendString("on\n");  }
				} else {
					USART1_SendString(buff); // If new data received, send data back
					USART1_SendChar('\n');
				}
			}

			DWT_nb_delay_ms(&usart1_d1, 50);
		}

		// non blocking delay, DHT11
		if (DWT_nb_timeout(&dht11_d1)) {
			if (DHT11_read(&dht11)) {
				sprintf(dht_tx_buffer, "%d,%d\n", dht11.temperature, dht11.humidity);
				USART1_SendString(dht_tx_buffer);
			}
			DWT_nb_delay_ms(&dht11_d1, 2000);
		}
	}

	return 0;
}
