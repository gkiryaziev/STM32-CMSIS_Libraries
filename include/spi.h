/*
 * spi.h
 *
 * Updated on: 29.05.2025
 * Created on: 09.05.2019
 *     Author: Admin
 * 
 * SPI1
 * NSS : PA4 (General purpose output push-pull)
 * SCK : PA5 (Alternate function push-pull)
 * MISO: PA6 (Input floating / Input pull-up)
 * MOSI: PA7 (Alternate function push-pull)
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
