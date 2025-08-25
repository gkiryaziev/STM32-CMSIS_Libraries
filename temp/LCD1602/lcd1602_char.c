/*
 * lcd1602_char.c
 *
 *  Updated on: 01.06.2025
 *  Created on: 28.05.2019
 *      Author: Admin
 */

#include "lcd1602_char.h"


const uint8_t SMILE[8]   = { 0b00000, 0b01010, 0b11011, 0b01110, 0b00100, 0b10001, 0b01110, 0b00000 };
const uint8_t CHEST[8]   = { 0b00100, 0b01110, 0b11011, 0b01110, 0b00100, 0b00100, 0b11111, 0b00100 };
const uint8_t HUMAN[8]   = { 0b11111, 0b01110, 0b00100, 0b11111, 0b00100, 0b00100, 0b01010, 0b10001 };
const uint8_t HOUSE[8]   = { 0b00100, 0b01010, 0b10001, 0b11111, 0b10001, 0b10101, 0b10001, 0b11111 };
const uint8_t BATTARY[8] = { 0b00000, 0b01110, 0b11111, 0b10001, 0b10001, 0b11111, 0b11111, 0b11111 };
const uint8_t ANTENNA[8] = { 0b00000, 0b11111, 0b10001, 0b01110, 0b00100, 0b00100, 0b00100, 0b00100 };
const uint8_t ARROWUP[8] = { 0b00100, 0b01110, 0b11111, 0b00100, 0b00100, 0b00100, 0b00100, 0b00100 };

const uint8_t* const lcd1602CustomChars[] = {
    SMILE,
    CHEST,
    HUMAN,
    HOUSE,
    BATTARY,
    ANTENNA,
    ARROWUP
};
