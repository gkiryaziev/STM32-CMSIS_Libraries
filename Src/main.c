#include "main.h"

uint8_t state1, state2 = 0;
Delay_TypeDef d1, d2, usart1_d1;
char buffer[USART1_BUFFER_SIZE] = {0};		// Set buffer size in usart.h file


int main(void)
{
	RCC_Init();
	DWT_Init();
	GPIO_Init();
	USART1_Init();

	// for two LEDs
	GPIO_Enable(GPIOB);
	GPIO_SetMode_Output_50MHz_PP(GPIOB, 12);
	GPIO_SetMode_Output_50MHz_PP(GPIOB, 14);

	for(;;) {

//		// blocking delay
//		GPIO_PC13_On();
//		DWT_delay_ms(400);
//		GPIO_PC13_Off();
//		DWT_delay_ms(300);

		// non blocking delay, LED1
		if (DWT_nb_timeout(&d1)) {
			GPIO_WritePin(GPIOB, 12 , state1); state1 = !state1;
			DWT_nb_delay_ms(&d1, 300);
		}

		// non blocking delay, LED2
		if (DWT_nb_timeout(&d2)) {
			GPIO_WritePin(GPIOB, 14 , state2); state2 = !state2;
			DWT_nb_delay_ms(&d2, 700);
		}

		// non blocking delay, USART1
		if (DWT_nb_timeout(&usart1_d1)) {
			if (USART1_ReadString(buffer))
				USART1_SendString(buffer);	// If new data received, send data back
			DWT_nb_delay_ms(&usart1_d1, 50);
		}
	}
}
