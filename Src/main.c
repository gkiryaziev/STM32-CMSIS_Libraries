#include "main.h"

#define LED1 12
#define LED2 14

uint8_t led1_s = 0, led2_s = 0;                  // states
Delay_TypeDef led1_d, led2_d, usart1_d, dht11_d; // delays
char usart_buffer[USART1_BUFFER_SIZE];           // set buffer size in usart.h file
DHT11_TypeDef dht11;                             // DHT structure

// Encoder 0
Delay_TypeDef encoder0_d;
RotaryEncoder_TypeDef encoder0;
int8_t encoder0_count, encoder0_value;

void usart1_process() {
	switch (usart_buffer[0]) {
	case 'L':                                    // PC13
		if (usart_buffer[1] == '?') {
			char pc13_buffer[16];
			sprintf(pc13_buffer, "L:%d\n", GPIO_ReadPin(GPIOC, 13));
			USART1_SendString(pc13_buffer);
		} else if (usart_buffer[1] == '=') {
			if (usart_buffer[2] == '0') {
				GPIO_PC13_Off();
				USART1_SendString("L:0\n");
			}
			if (usart_buffer[2] == '1') {
				GPIO_PC13_On();
				USART1_SendString("L:1\n");
			}
		}
		break;
	case 'D':                                    // DHT
		if (usart_buffer[1] == '?') {
		}
		break;
	default:
		USART1_SendString(usart_buffer);         // Send data back
		USART1_SendChar('\n');
		break;
	}
}

int main() {

	RCC_Init();
	DWT_Init();
	GPIO_Init();
	USART1_Init();

	// Encoders
	RotaryEncoder_Init(&encoder0, GPIOB, 3, GPIOB, 4);	// encoder, clk port/pin, dt port/pin

	// for two LEDs
	GPIO_Enable(GPIOB);
	GPIO_SetMode_Output_2MHz_PP(GPIOB, LED1);
	GPIO_SetMode_Output_10MHz_PP(GPIOB, LED2);

	for (;;) {

		// non blocking delay, LED1
		if (DWT_nb_timeout(&led1_d)) {
			GPIO_WritePin(GPIOB, LED1, led1_s);
			led1_s = !led1_s;
			DWT_nb_delay_ms(&led1_d, 300);
		}

		// non blocking delay, LED2
		if (DWT_nb_timeout(&led2_d)) {
			GPIO_WritePin(GPIOB, LED2, led2_s);
			led2_s = !led2_s;
			DWT_nb_delay_ms(&led2_d, 750);
		}

		// non blocking delay, USART1
		if (DWT_nb_timeout(&usart1_d)) {
			if (USART1_ReadString(usart_buffer)) {
				usart1_process();
			}
			DWT_nb_delay_ms(&usart1_d, 50);
		}

		// non blocking delay, DHT11
		if (DWT_nb_timeout(&dht11_d)) {
			if (DHT11_read(&dht11)) {
				char dht_buffer[16];
				sprintf(dht_buffer, "%dDHT%d\n", dht11.temperature,
						dht11.humidity);
				USART1_SendString(dht_buffer);
			}
			DWT_nb_delay_ms(&dht11_d, 2000);
		}

		// non blocking delay, Rotary Encoder
		if (DWT_nb_timeout(&encoder0_d)) {
			if ((encoder0_value = RotaryEncoder_Read(&encoder0))) {
				encoder0_count += encoder0_value;
				char enc_buffer[20];
				if (encoder0.current_state == 0xb) {
					sprintf(enc_buffer, "%d - CW - %02x\n", encoder0_count,
							encoder0.transition);
					USART1_SendString(enc_buffer);
				}
				if (encoder0.current_state == 0x7) {
					sprintf(enc_buffer, "%d - CCW - %02x\n", encoder0_count,
							encoder0.transition);
					USART1_SendString(enc_buffer);
				}
			}
			DWT_nb_delay_ms(&encoder0_d, 1);
		}
	}

	return 0;
}
