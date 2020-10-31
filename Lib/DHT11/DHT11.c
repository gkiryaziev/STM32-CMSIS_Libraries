/*
 * DHT11.c
 */

#include "DHT11.h"

// -----------------------------------------------
// private functions
// -----------------------------------------------
uint8_t DHT_start(void);
uint8_t DHT_read_byte(void);

// -----------------------------------------------
// DHT Send start command
// -----------------------------------------------
uint8_t DHT_start() {
	// start
	GPIO_Enable(DHT_PORT);

	GPIO_SetMode_Output_2MHz_PP(DHT_PORT, DHT_PIN);

	GPIO_ResetPin(DHT_PORT, DHT_PIN);

	DWT_delay_ms(18);

	GPIO_SetPin(DHT_PORT, DHT_PIN);

	DWT_delay_us(40);

	// check response
	GPIO_SetMode_Input_Floating(DHT_PORT, DHT_PIN);

	if (GPIO_ReadPin(DHT_PORT, DHT_PIN))
		return 0;

	DWT_delay_us(80);

	if (!GPIO_ReadPin(DHT_PORT, DHT_PIN))
		return 0;

	while (GPIO_ReadPin(DHT_PORT, DHT_PIN));

	return 1;
}

// -----------------------------------------------
// DHT Read byte
// -----------------------------------------------
uint8_t DHT_read_byte() {

	uint8_t data = 0, j;

	for (j = 0; j < 8; ++j) {

		while (!GPIO_ReadPin(DHT_PORT, DHT_PIN));

		DWT_delay_us(40);

		if (GPIO_ReadPin(DHT_PORT, DHT_PIN)){
			data |= (1 << (7 - j));
			while(GPIO_ReadPin(DHT_PORT, DHT_PIN));
		}
	}
	return data;
}

// -----------------------------------------------
// DHT Read data
// -----------------------------------------------
uint8_t DHT_read(DHT11_TypeDef *dht11) {

	if (DHT_start()) {

		uint8_t rh_b1 = DHT_read_byte();
		uint8_t rh_b2 = DHT_read_byte();
		uint8_t temp_b1 = DHT_read_byte();
		uint8_t temp_b2 = DHT_read_byte();
		uint16_t sum = DHT_read_byte();

		if (sum == (rh_b1 + rh_b2 + temp_b1 + temp_b2)) {
			dht11->humidity = rh_b1;
			dht11->temperature = temp_b1;
			return 1;
		}
	}

	return 0;
}
