/*
 * RotaryEncoder.h
 *
 * Created on: 10.12.2021
 *     Author: Admin
 */

#ifndef ROTARY_ENCODER_H_
#define ROTARY_ENCODER_H_

#ifdef __cplusplus
extern "C" {
#endif /* __cplusplus */

#include "stm32f1xx.h"
#include "gpio.h"

// -----------------------------------------------
// public struct
// -----------------------------------------------
typedef struct {
	GPIO_TypeDef *clk_port;
	uint8_t clk_pin;
	GPIO_TypeDef *dt_port;
	uint8_t dt_pin;
	uint8_t current_state;
	uint16_t transition;
} RotaryEncoder_TypeDef;

// -----------------------------------------------
// public functions
// -----------------------------------------------
void RotaryEncoder_Init(RotaryEncoder_TypeDef *encoder, GPIO_TypeDef *clk_port,
		uint8_t clk_pin, GPIO_TypeDef *dt_port, uint8_t dt_pin);
int8_t RotaryEncoder_Read(RotaryEncoder_TypeDef *encoder);

#ifdef __cplusplus
}
#endif /* __cplusplus */

#endif /* ROTARY_ENCODER_H_ */
