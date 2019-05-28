/*
 * HC-SR04.h
 *
 *  Created on: 26 мар. 2019 г.
 *      Author: Admin
 */

#ifndef HCSR04_H_
#define HCSR04_H_

#ifdef __cplusplus
extern "C" {
#endif /* __cplusplus */

#include "stm32f1xx.h"
#include "delay.h"

// -----------------------------------------------
// public functions
// -----------------------------------------------
void HCSR04_Init(void);
uint32_t HCSR04_Read(void);

void HCSR04_InitIT(void);
uint32_t HCSR04_ReadIT(void);

void HCSR04_InitPWM(void);
uint32_t HCSR04_ReadPWM(void);
// -----------------------------------------------

#ifdef __cplusplus
}
#endif /* __cplusplus */

#endif /* HCSR04_H_ */
