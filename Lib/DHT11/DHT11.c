/*
 * DHT11.c
 */

#include "DHT11.h"

// -----------------------------------------------
// private functions
// -----------------------------------------------
uint8_t DHT11_start(void);
uint8_t DHT11_read_byte(void);

// -----------------------------------------------
// DHT Send start command
// -----------------------------------------------
uint8_t DHT11_start() {
	// start
	GPIO_Enable(DHT11_PORT);

	GPIO_SetMode_Output_2MHz_PP(DHT11_PORT, DHT11_PIN);

	GPIO_ResetPin(DHT11_PORT, DHT11_PIN);

	DWT_delay_ms(18);

	GPIO_SetPin(DHT11_PORT, DHT11_PIN);

	DWT_delay_us(40);

	// check response
	GPIO_SetMode_Input_Floating(DHT11_PORT, DHT11_PIN);

	if (GPIO_ReadPin(DHT11_PORT, DHT11_PIN))
		return 0;

	DWT_delay_us(80);

	if (!GPIO_ReadPin(DHT11_PORT, DHT11_PIN))
		return 0;

	while (GPIO_ReadPin(DHT11_PORT, DHT11_PIN));

	return 1;
}

// -----------------------------------------------
// DHT Read byte
// -----------------------------------------------
uint8_t DHT11_read_byte() {

	uint8_t data = 0, j;

	for (j = 0; j < 8; ++j) {

		while (!GPIO_ReadPin(DHT11_PORT, DHT11_PIN));

		DWT_delay_us(40);

		if (GPIO_ReadPin(DHT11_PORT, DHT11_PIN)){
			data |= (1 << (7 - j));
			while(GPIO_ReadPin(DHT11_PORT, DHT11_PIN));
		}
	}
	return data;
}

// -----------------------------------------------
// DHT Read all data
// -----------------------------------------------
uint8_t DHT11_read(DHT11_TypeDef *dht11) {

	if (DHT11_start()) {

		uint8_t hum_b1 = DHT11_read_byte();
		uint8_t hum_b2 = DHT11_read_byte();
		uint8_t tmp_b1 = DHT11_read_byte();
		uint8_t tmp_b2 = DHT11_read_byte();
		uint16_t c_sum = DHT11_read_byte();

		if (c_sum == (hum_b1 + hum_b2 + tmp_b1 + tmp_b2)) {
			dht11->humidity = hum_b1;
			dht11->temperature = tmp_b1;
			return 1;
		}
	}

	return 0;
}
