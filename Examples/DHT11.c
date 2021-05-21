#include "main.h"

Delay_TypeDef dht11_d; // delays
DHT11_TypeDef dht11;   // DHT structure

int main() {

	RCC_Init();
	DWT_Init();
	GPIO_Init();
	USART1_Init();

	for (;;) {

		// non blocking delay, DHT11
		if (DWT_nb_timeout(&dht11_d)) {
			if (DHT11_read(&dht11)) {
				char dht_buffer[16];
				sprintf(dht_buffer, "T:%d, H:%d\n", dht11.temperature, dht11.humidity);
				USART1_SendString(dht_buffer);
			}
			DWT_nb_delay_ms(&dht11_d, 2000);
		}
	}

	return 0;
}
