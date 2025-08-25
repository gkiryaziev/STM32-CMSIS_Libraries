/*
 * lcd1602_char.h
 *
 *  Updated on: 01.06.2025
 *  Created on: 28.05.2019
 *      Author: Admin
 */

#ifndef LCD1602_CHAR_H_
#define LCD1602_CHAR_H_

#ifdef __cplusplus
extern "C" {
#endif

#include <stdint.h>

typedef enum {
    LCD1602_SMILE = 0x0,
    LCD1602_CHEST,
    LCD1602_HUMAN,
    LCD1602_HOUSE,
    LCD1602_BATTARY,
    LCD1602_ANTENNA,
    LCD1602_ARROWUP
} LCD1602_CustomChar;

extern const uint8_t SMILE[8];
extern const uint8_t CHEST[8];
extern const uint8_t HUMAN[8];
extern const uint8_t HOUSE[8];
extern const uint8_t BATTARY[8];
extern const uint8_t ANTENNA[8];
extern const uint8_t ARROWUP[8];

extern const uint8_t* const lcd1602CustomChars[];

#ifdef __cplusplus
}
#endif

#endif /* LCD1602_CHAR_H_ */
