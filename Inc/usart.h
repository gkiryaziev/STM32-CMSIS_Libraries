/*
 * USART1
 * TX        - PA9
 * RX        - PA10
 */
#ifndef __USART_H_
#define __USART_H_

#ifdef __cplusplus
extern "C" {
#endif /* __cplusplus */

#include "stm32f1xx.h"

#define APB2_CLK          ((uint32_t)72000000)
#define USART1_BAUD_RATE  ((uint32_t)115200)

void USART1_Init();
void USART1_SendChar(char chr);
void USART1_SendString(char *str);

#ifdef __cplusplus
}
#endif /* __cplusplus */

#endif /* __USART_H_ */

