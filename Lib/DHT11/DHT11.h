/*
 * DHT11.h
 *
 *  Created on: 24 мар. 2019 г.
 *      Author: Admin
 */
#ifndef DHT11_H_
#define DHT11_H_

#ifdef __cplusplus
extern "C" {
#endif /* __cplusplus */

#include "stm32f1xx.h"
#include "delay.h"

// -----------------------------------------------
// public struct
// -----------------------------------------------
typedef struct {
	uint8_t humidity;
	uint8_t temperature;
} DHT11_TypeDef;
// -----------------------------------------------

// -----------------------------------------------
// public functions
// -----------------------------------------------
uint8_t DHT_read(DHT11_TypeDef *dht11);
// -----------------------------------------------

#ifdef __cplusplus
}
#endif /* __cplusplus */

#endif /* DHT11_H_ */
