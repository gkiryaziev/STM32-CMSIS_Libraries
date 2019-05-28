/*
 * rtc.h
 *
 *  Created on: 26 апр. 2019 г.
 *      Author: Admin
 */

#ifndef RTC_H_
#define RTC_H_

#ifdef __cplusplus
extern "C" {
#endif /* __cplusplus */

#include "stm32f1xx.h"

typedef struct {
    uint8_t Hours;
    uint8_t Minutes;
    uint8_t Seconds;
} RTC_TimeTypeDef;

void RTC_Init(void);
void RTC_SetCounter(uint32_t count);
uint32_t RTC_GetCounter(void);
void RTC_SetTime(RTC_TimeTypeDef *time);
void RTC_GetTime(RTC_TimeTypeDef *time);

#ifdef __cplusplus
}
#endif /* __cplusplus */

#endif /* RTC_H_ */
