/*
 * main.h
 *
 * Created on: 03.06.2020
 *     Author: Admin
 */

#ifndef MAIN_H_
#define MAIN_H_

#ifdef __cplusplus
extern "C" {
#endif /* __cplusplus */

#include "stm32f1xx.h"

#include <stdio.h>
#include <string.h>

#include "delay.h"
#include "gpio.h"
#include "rcc.h"
#include "usart.h"

#include "lcd1602_i2c.h"
#include "lcd1602_char.h"    // custom char
#include "led_display.h"

#ifdef __cplusplus
}
#endif /* __cplusplus */

#endif /* MAIN_H_ */
