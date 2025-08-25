/*
 * usart.c
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

#include "usart.h"

volatile char usart1_buffer[USART1_BUFFER_SIZE];
volatile uint16_t usart1_head = 0;
volatile uint16_t usart1_tail = 0;

// -----------------------------------------------
// USART1_Init
// -----------------------------------------------
void USART1_Init(void) {

    RCC->APB2ENR |= RCC_APB2ENR_USART1EN;          // 1: USART1 clock enabled
    RCC->APB2ENR |= RCC_APB2ENR_IOPAEN;            // 1: I/O port A clock enabled
    RCC->APB2ENR |= RCC_APB2ENR_AFIOEN;            // 1: Alternate Function I/O clock enabled

    // TX alternative push-pull (PA9)
    GPIOA->CRH |= GPIO_CRH_MODE9;                  // 11 Output mode, max speed 50 MHz.
    GPIOA->CRH &= ~GPIO_CRH_CNF9;                  // 00 CNF9 reset
    GPIOA->CRH |= GPIO_CRH_CNF9_1;                 // 10 Alternate function output Push-pull

    // RX float input (PA10)
    GPIOA->CRH &= ~GPIO_CRH_MODE10;                // 00 Input mode (reset state)
    GPIOA->CRH &= ~GPIO_CRH_CNF10;                 // 00 CNF10 reset
    GPIOA->CRH |= GPIO_CRH_CNF10_0;                // 01 Floating input (reset state)

    USART1->BRR = APB2_CLK / USART1_BAUD_RATE;     // USARTx_BRR = APB2CLK / USART1_BAUD_RATE
    USART1->CR1 |= USART_CR1_UE;                   // USART enable
    USART1->CR1 |= USART_CR1_TE;                   // Transmitter enable
    USART1->CR1 |= USART_CR1_RE;                   // Receiver enable

    USART1->CR1 |= USART_CR1_RXNEIE;               // RXNE interrupt enable
    NVIC_EnableIRQ(USART1_IRQn);                   // USART1 global Interrupt
}

// -----------------------------------------------
// USART1_SendChar
// -----------------------------------------------
void USART1_SendChar(char ch) {

    while (!(USART1->SR & USART_SR_TXE));          // 0: Data is not transferred to the shift register
    USART1->DR = ch;
}

// -----------------------------------------------
// USART1_SendString
// -----------------------------------------------
void USART1_SendString(char *str) {

    while (*str) {
        USART1_SendChar(*str++);
    }
}

// -----------------------------------------------
// USART1_Available (number of available bytes)
// -----------------------------------------------
uint16_t USART1_Available(void) {
    uint16_t available_bytes;
    uint16_t local_head;
    uint16_t local_tail;

    __disable_irq();
    local_head = usart1_head;
    local_tail = usart1_tail;
    __enable_irq();

    if (local_head >= local_tail) {
        available_bytes = local_head - local_tail;
    } else {
        available_bytes = USART1_BUFFER_SIZE - (local_tail - local_head);
    }

    return available_bytes;
}

// -----------------------------------------------
// USART1_Read (reading an arbitrary number of characters)
// -----------------------------------------------
uint16_t USART1_Read(char *buffer, uint16_t max_len) {
    
    uint16_t bytes_read = 0;
    uint16_t local_head;
    uint16_t local_tail;

    if (max_len == 0 || buffer == NULL) {
        return 0;
    }

    __disable_irq();
    local_head = usart1_head;
    local_tail = usart1_tail;
    __enable_irq();

    while ((local_tail != local_head) && (bytes_read < max_len)) {
        buffer[bytes_read++] = usart1_buffer[local_tail];
        local_tail = (local_tail + 1) & USART1_BUFFER_MASK;
    }
    
    if (bytes_read > 0) {
        __disable_irq();
        usart1_tail = local_tail;
        __enable_irq();
    }

    return bytes_read;
}

// -----------------------------------------------
// USART1_ReadString (line reading up to the terminator)
// -----------------------------------------------
uint16_t USART1_ReadString(char *buffer) {
    
    uint16_t bytes_read = 0;
    uint16_t current_tail;
    uint16_t local_head;

    if (buffer == NULL) {
        return 0;
    }

    __disable_irq();
    local_head = usart1_head;
    current_tail = usart1_tail;
    __enable_irq();

    if (current_tail == local_head) {
        return 0;
    }

    while (current_tail != local_head && bytes_read < (USART1_BUFFER_SIZE - 1)) {
        
        char c = usart1_buffer[current_tail];
        current_tail = (current_tail + 1) & USART1_BUFFER_MASK;

        if (c == '\n' || c == '\r') {
            buffer[bytes_read] = '\0';
            
            __disable_irq();
            usart1_tail = current_tail;
            __enable_irq();
            
            return bytes_read;
        }
        buffer[bytes_read++] = c;
    }

    if (bytes_read > 0) {
        buffer[bytes_read] = '\0';
        
        __disable_irq();
        usart1_tail = current_tail;
        __enable_irq();
    }
    
    return bytes_read;
}

// -----------------------------------------------
// USART1_IRQHandler
// -----------------------------------------------
void USART1_IRQHandler() {

    if (USART1->SR & USART_SR_RXNE) {              // 1: Received data is ready to be read
        
        char rx_char = (char)USART1->DR;

        if (((usart1_head + 1) & USART1_BUFFER_MASK) != usart1_tail) {
            usart1_buffer[usart1_head] = rx_char;
            usart1_head = (usart1_head + 1) & USART1_BUFFER_MASK;
        }
    }
}
