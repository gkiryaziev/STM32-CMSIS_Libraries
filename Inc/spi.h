/*
 * spi.h
 *
 * Created on: 9.05.2019
 *     Author: Admin
 */

#ifndef SPI_H_
#define SPI_H_

#ifdef __cplusplus
extern "C" {
#endif /* __cplusplus */

#include "stm32f1xx.h"

//#define SPI1_16_BIT_FORMAT

void SPI1_Init(void);
void SPI1_EnableSlave(void);
void SPI1_DisableSlave(void);
uint16_t SPI1_Write(uint16_t data);

#ifdef __cplusplus
}
#endif /* __cplusplus */

#endif /* SPI_H_ */
