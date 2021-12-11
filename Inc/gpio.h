/*
 * gpio.h
 *
 * Updated on: 03.06.2020
 * Created on: 13.11.2018
 *     Author: Admin
 */

#ifndef GPIO_H_
#define GPIO_H_

#ifdef __cplusplus
extern "C" {
#endif /* __cplusplus */

#include "stm32f1xx.h"

void GPIO_Init();
void GPIO_SetPin(GPIO_TypeDef *GPIOx, uint8_t pin);
void GPIO_ResetPin(GPIO_TypeDef *GPIOx, uint8_t pin);
void GPIO_WritePin(GPIO_TypeDef *GPIOx, uint8_t pin, uint8_t state);
uint16_t GPIO_ReadPin(GPIO_TypeDef *GPIOx, uint8_t pin);
void GPIO_Enable(GPIO_TypeDef *GPIOx);
void GPIO_Disable_JTAG(void);
void GPIO_SetMode_Output_2MHz_PP(GPIO_TypeDef *GPIOx, uint8_t pin);
void GPIO_SetMode_Output_10MHz_PP(GPIO_TypeDef *GPIOx, uint8_t pin);
void GPIO_SetMode_Output_50MHz_PP(GPIO_TypeDef *GPIOx, uint8_t pin);
void GPIO_SetMode_Input_Floating(GPIO_TypeDef *GPIOx, uint8_t pin);
void GPIO_SetMode_Input_PullDown(GPIO_TypeDef *GPIOx, uint8_t pin);
void GPIO_SetMode_Input_PullUp(GPIO_TypeDef *GPIOx, uint8_t pin);

// Blue Pill Led, PC13
void GPIO_PC13_On();
void GPIO_PC13_Off();

#ifdef __cplusplus
}
#endif /* __cplusplus */

#endif /* GPIO_H_ */
