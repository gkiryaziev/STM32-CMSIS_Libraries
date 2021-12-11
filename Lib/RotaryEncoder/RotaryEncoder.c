/*
 * RotaryEncoder.c
 */

#include "RotaryEncoder.h"

volatile int8_t encoder_table[] = { 0, 1, 1, 0, 1, 0, 0, 1, 1, 0, 0, 1, 0, 1, 1,
		0 };

// -----------------------------------------------
// Rotary Encoder Init
// -----------------------------------------------
void RotaryEncoder_Init(RotaryEncoder_TypeDef *encoder, GPIO_TypeDef *clk_port,
		uint8_t clk_pin, GPIO_TypeDef *dt_port, uint8_t dt_pin) {

	encoder->clk_port = clk_port;
	encoder->clk_pin = clk_pin;
	encoder->dt_port = dt_port;
	encoder->dt_pin = dt_pin;

	GPIO_Enable(clk_port);
	GPIO_Enable(dt_port);

	GPIO_SetMode_Input_Floating(clk_port, clk_pin);
	GPIO_SetMode_Input_Floating(dt_port, dt_pin);
}

// -----------------------------------------------
// Rotary Encoder Read
// -----------------------------------------------
int8_t RotaryEncoder_Read(RotaryEncoder_TypeDef *encoder) {

	encoder->current_state <<= 2;
	if (GPIO_ReadPin(encoder->dt_port, encoder->dt_pin))
		encoder->current_state |= 0x2;
	if (GPIO_ReadPin(encoder->clk_port, encoder->clk_pin))
		encoder->current_state |= 0x1;
	encoder->current_state &= 0xf;

	if (encoder_table[encoder->current_state]) {
		encoder->transition <<= 4;
		encoder->transition |= encoder->current_state;
		if ((encoder->transition & 0xff) == 0x2b)			// 0010 -> 1011
			return 1;
		if ((encoder->transition & 0xff) == 0x17)			// 0001 -> 0111
			return -1;
		//if (encoder->transition==0xd42b) return 1;		// 11010100 -> 00101011
		//if (encoder->transition==0xe817) return -1;		// 11101000 -> 00010111
	}
	return 0;
}
