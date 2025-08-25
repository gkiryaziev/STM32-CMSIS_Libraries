/*
 * 74HC595.c
 *
 * Updated on: 09.06.2025
 * Created on: 19.05.2019
 *     Author: Admin
 *
 * LATCH    - PB12 (General output PP)
 * CLOCK    - PB13 (General output PP)
 * DATA     - PB14 (General output PP)
 *
 * NSS      - PA4 (General purpose output push-pull) (not used)
 * SCK      - PA5 (Alternate function push-pull)
 * MISO     - PA6 (Input floating / Input pull-up)   (not used)
 * MOSI     - PA7 (Alternate function push-pull)
 */

#include "74HC595.h"


// -----------------------------------------------
// HC595_SPI_Init
// -----------------------------------------------
void HC595_SPI_Init(void) {

    GPIO_Enable(LATCH_Port);                             // Enable port
    GPIO_SetMode_Output_2MHz_PP(LATCH_Port, LATCH_Pin);  // Init latch pin
    GPIO_WritePin(LATCH_Port, LATCH_Pin, 0);             // Low level
}

// -----------------------------------------------
// HC595_SPI_SendByte
// -----------------------------------------------
void HC595_SPI_SendByte(uint8_t byte) {

    SPI1_Write(byte);

    // Toggle latch pin
    HC595_Latch();
}

// -----------------------------------------------
// HC595_SPI_SendBuffer
// -----------------------------------------------
void HC595_SPI_SendBuffer(const uint8_t *data, uint8_t length) {
    
    for (uint8_t i = 0; i < length; ++i) {
        SPI1_Write(data[i]);
    }

    // Toggle latch pin
    HC595_Latch();
}

// -----------------------------------------------
// HC595_BitBang_Init
// -----------------------------------------------
void HC595_BitBang_Init(void){

    GPIO_Enable(LATCH_Port);

    GPIO_SetMode_Output_2MHz_PP(CLOCK_Port, CLOCK_Pin);
    GPIO_SetMode_Output_2MHz_PP(LATCH_Port, LATCH_Pin);
    GPIO_SetMode_Output_2MHz_PP(DATA_Port, DATA_Pin);

    GPIO_WritePin(CLOCK_Port, CLOCK_Pin, 0);
    GPIO_WritePin(LATCH_Port, LATCH_Pin, 0);
    GPIO_WritePin(DATA_Port, DATA_Pin, 0);
}

// -----------------------------------------------
// HC595_BitBang_SendByte
// -----------------------------------------------
void HC595_BitBang_SendByte(uint8_t byte){

    for (int8_t i = 7; i >= 0; --i) {

        GPIO_WritePin(DATA_Port, DATA_Pin, (byte & (0x1 << i)) ? 1 : 0); // Send bit

        // Toggle clock
        GPIO_WritePin(CLOCK_Port, CLOCK_Pin, 1);
        DWT_delay_us(1);
        GPIO_WritePin(CLOCK_Port, CLOCK_Pin, 0);
    }

    // Toggle latch
    HC595_Latch();
}

// -----------------------------------------------
// HC595_Latch
// -----------------------------------------------
void HC595_Latch(void) {

    GPIO_WritePin(LATCH_Port, LATCH_Pin, 1);
    DWT_delay_us(1);
    GPIO_WritePin(LATCH_Port, LATCH_Pin, 0);
}
