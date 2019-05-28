/*
 * led_display.c
 *
 *  Created on: 22.05.2019
 *      Author: Admin
 */

#include "led_display.h"

// Digits
uint8_t LED_Display_Digits[] = {0x3F, 0x06, 0x5B, 0x4F, 0x66, 0x6D, 0x7D, 0x07, 0x7F, 0x6F};
uint8_t LED_Display_Buffer[2] = {0};
uint8_t LED_Display_Current_Digit = 0;

void LED_Display_Init(void){

	GPIO_Enable(LED_DIG1_Port);									// Both pins have the same port

	GPIO_SetMode_Output_2MHz_PP(LED_DIG1_Port, LED_DIG1_Pin);
	GPIO_SetMode_Output_2MHz_PP(LED_DIG2_Port, LED_DIG2_Pin);
	GPIO_WritePin(LED_DIG1_Port, LED_DIG1_Pin, 0);
	GPIO_WritePin(LED_DIG2_Port, LED_DIG2_Pin, 0);

	// TIM2
	RCC->APB1ENR |= RCC_APB1ENR_TIM2EN;							// TIM2EN: TIM2 timer clock enable
	TIM2->PSC = (SystemCoreClock / 1000000) - 1;				// PSC[15:0]: Prescaler value, 1uS tick.
	TIM2->ARR = 5000 - 1;										// ARR[15:0]: Prescaler value, 5mS
	TIM2->DIER |= TIM_DIER_UIE;									// 1: Update interrupt enabled.
	TIM2->CR1 |= TIM_CR1_CEN;									// 1: Counter enabled
	NVIC_EnableIRQ(TIM2_IRQn);									// Enable External Interrupt, TIM2 global Interrupt
}

void LED_Display_Write(uint8_t data) {

	LED_Display_Buffer[0] = data / 10;
	LED_Display_Buffer[1] = data % 10;
}

void TIM2_IRQHandler()
{
	TIM2->SR &= ~TIM_SR_UIF;									// 0: No update occurred.

	if (LED_Display_Current_Digit == 0) {
		GPIO_WritePin(LED_DIG1_Port, LED_DIG1_Pin, 1);			// DIG1 On
		GPIO_WritePin(LED_DIG2_Port, LED_DIG2_Pin, 0);			// DIG1 Off
		HC595_SendByte(LED_Display_Digits[LED_Display_Buffer[LED_Display_Current_Digit]]);
	}

	if (LED_Display_Current_Digit == 1) {
		GPIO_WritePin(LED_DIG1_Port, LED_DIG1_Pin, 0);			// DIG1 Off
		GPIO_WritePin(LED_DIG2_Port, LED_DIG2_Pin, 1);			// DIG1 On
		HC595_SendByte(LED_Display_Digits[LED_Display_Buffer[LED_Display_Current_Digit]]);
	}

	LED_Display_Current_Digit++;

	if (LED_Display_Current_Digit > 2) { LED_Display_Current_Digit = 0; }
}
