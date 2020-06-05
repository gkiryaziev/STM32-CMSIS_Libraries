#include "main.h"

uint8_t state1, state2 = 0;
Delay_TypeDef d1, d2;

int main(void)
{
	RCC_Init();
	DWT_Init();
	GPIO_Init();

	GPIO_Enable(GPIOB);
	GPIO_SetMode_Output_50MHz_PP(GPIOB, 12);
	GPIO_SetMode_Output_50MHz_PP(GPIOB, 14);

	for(;;) {

//		// blocking delay
//		GPIO_PC13_On();
//		DWT_delay_ms(400);
//		GPIO_PC13_Off();
//		DWT_delay_ms(300);

		// non blocking delay
		if (DWT_nb_timeout(&d1)) {
			GPIO_WritePin(GPIOB, 12 , state1); state1 = !state1;
			DWT_nb_delay_ms(&d1, 300);
		}

		if (DWT_nb_timeout(&d2)) {
			GPIO_WritePin(GPIOB, 14 , state2); state2 = !state2;
			DWT_nb_delay_ms(&d2, 700);
		}
	}
}
