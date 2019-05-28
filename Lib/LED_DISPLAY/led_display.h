/*
 * led_display.h
 *
 *  Created on: 22.05.2019
 *      Author: Admin
 */

#ifndef LED_DISPLAY_H_
#define LED_DISPLAY_H_

#ifdef __cplusplus
extern "C" {
#endif /* __cplusplus */

#include "stm32f1xx.h"
#include "gpio.h"
#include "74HC595.h"		// <-- set here pins for data line

#define LED_DIG1_Port		GPIOB
#define LED_DIG1_Pin		8
#define LED_DIG2_Port		GPIOB
#define LED_DIG2_Pin		9

void LED_Display_Init(void);
void LED_Display_Write(uint8_t data);

#ifdef __cplusplus
}
#endif /* __cplusplus */

#endif /* LED_DISPLAY_H_ */
