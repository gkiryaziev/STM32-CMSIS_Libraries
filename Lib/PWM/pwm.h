/*
 * pwm.h
 *
 *  Created on: 20 апр. 2019 г.
 *      Author: Admin
 */

#ifndef PWM_H_
#define PWM_H_

#include "stm32f1xx.h"

void PWM_Init(void);
void PWM_Set(uint32_t value);

#endif /* PWM_H_ */
