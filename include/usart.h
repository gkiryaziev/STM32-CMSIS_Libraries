/*
 * usart.h
 *
 * Updated on: 28.05.2025
 * Updated on: 15.05.2021
 * Updated on: 15.06.2020
 * Created on: 13.11.2018
 *     Author: Admin
 *
 * USART1
 * APB CLOCK: 72MHz
 * BAUD RATE: 115200
 * TX       : PA9
 * RX       : PA10
 */

#ifndef __USART_H_
#define __USART_H_

#ifdef __cplusplus
extern "C" {
#endif /* __cplusplus */

#include "stm32f1xx.h"

#ifndef NULL
    #define NULL ((void *)0)
#endif

#define USART1_BUFFER_SIZE    ((uint16_t)64)
#define USART1_BUFFER_MASK    (USART1_BUFFER_SIZE - 1)
#define APB2_CLK              ((uint32_t)72000000)
#define USART1_BAUD_RATE      ((uint32_t)115200)

void USART1_Init(void);
void USART1_SendChar(char chr);
void USART1_SendString(char *str);
uint16_t USART1_Read(char *buffer, uint16_t max_len);
uint16_t USART1_ReadString(char *buffer);
uint16_t USART1_Available(void);

#ifdef __cplusplus
}
#endif /* __cplusplus */

#endif /* __USART_H_ */

