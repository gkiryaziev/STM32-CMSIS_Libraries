/*
 * BMP280.h
 *
 *  Created on: 04.05.2019
 *      Author: Admin
 */

#ifndef BMP280_H_
#define BMP280_H_

#include "i2c.h"
#include "delay.h"

#define BMP280_ADDRESS						0x76

// Preset Meas for Normal mode
#define BMP280_PRESET_CTRL_MEAS_NORMAL		(BMP280_CTRL_MEAS_OSRS_T_X16 | BMP280_CTRL_MEAS_OSRS_P_X16 | BMP280_CTRL_MEAS_MODE_NORMAL)
// Preset Meas for Forced mode
#define BMP280_PRESET_CTRL_MEAS_FORCED		(BMP280_CTRL_MEAS_OSRS_T_X16 | BMP280_CTRL_MEAS_OSRS_P_X16 | BMP280_CTRL_MEAS_MODE_FORCED)
// Preset Config
#define BMP280_PRESET_CONFIG				(BMP280_CONFIG_T_SB_125 | BMP280_CONFIG_FILTER_COEFF_2 | BMP280_CONFIG_SPI3W_OFF)

// ---------------------------------------------

#define BMP280_RESET_VALUE					0xB6

// Register
#define BMP280_REGISTER_ID					0xD0
#define BMP280_REGISTER_RESET				0xE0
#define BMP280_REGISTER_STATUS				0xF3
#define BMP280_REGISTER_CTRL_MEAS			0xF4
#define BMP280_REGISTER_CONFIG				0xF5
#define BMP280_REGISTER_PRESSURE_MSB		0xF7
#define BMP280_REGISTER_PRESSURE_LSB		0xF8
#define BMP280_REGISTER_PRESSURE_XLSB		0xF9
#define BMP280_REGISTER_TEMPERATURE_MSB		0xFA
#define BMP280_REGISTER_TEMPERATURE_LSB		0xFB
#define BMP280_REGISTER_TEMPERATURE_XLSB	0xFC

// Compensation parameter storage
#define BMP280_DIG_T1_LSB					0x88
#define BMP280_DIG_T1_MSB					0x89
#define BMP280_DIG_T2_LSB					0x8A
#define BMP280_DIG_T2_MSB					0x8B
#define BMP280_DIG_T3_LSB					0x8C
#define BMP280_DIG_T3_MSB					0x8D
#define BMP280_DIG_P1_LSB					0x8E
#define BMP280_DIG_P1_MSB					0x8F
#define BMP280_DIG_P2_LSB					0x90
#define BMP280_DIG_P2_MSB					0x91
#define BMP280_DIG_P3_LSB					0x92
#define BMP280_DIG_P3_MSB					0x93
#define BMP280_DIG_P4_LSB					0x94
#define BMP280_DIG_P4_MSB					0x95
#define BMP280_DIG_P5_LSB					0x96
#define BMP280_DIG_P5_MSB					0x97
#define BMP280_DIG_P6_LSB					0x98
#define BMP280_DIG_P6_MSB					0x99
#define BMP280_DIG_P7_LSB					0x9A
#define BMP280_DIG_P7_MSB					0x9B
#define BMP280_DIG_P8_LSB					0x9C
#define BMP280_DIG_P8_MSB					0x9D
#define BMP280_DIG_P9_LSB					0x9E
#define BMP280_DIG_P9_MSB					0x9F

/* ------------------------------------------------
 * Register 0xF4 "ctrl_meas"
 * ------------------------------------------------
 * osrs_t[7:5] | Temperature  | Typical temperature
 *             | oversampling | resolution
 * ------------------------------------------------
 *     000     |  skipped     |       -
 *     001     |    x1        | 16 bit / 0.0050 C
 *     010     |    x2        | 17 bit / 0.0025 C
 *     011     |    x4        | 18 bit / 0.0012 C
 *     100     |    x8        | 19 bit / 0.0006 C
 * 101 110 111 |    x16       | 20 bit / 0.0003 C
 * ------------------------------------------------
 */
#define BMP280_CTRL_MEAS_OSRS_T_SKIP		0x00
#define BMP280_CTRL_MEAS_OSRS_T_X1			0x20
#define BMP280_CTRL_MEAS_OSRS_T_X2			0x40
#define BMP280_CTRL_MEAS_OSRS_T_X4			0x60
#define BMP280_CTRL_MEAS_OSRS_T_X8			0x80
#define BMP280_CTRL_MEAS_OSRS_T_X16			0xA0


/* ---------------------------------------------
 * osrs_p[4:2] | Pressure     | Typical pressure
 *             | oversampling | resolution
 * ---------------------------------------------
 *     000     |  skipped     |       -
 *     001     |    x1        | 16 bit / 2.62 Pa
 *     010     |    x2        | 17 bit / 1.31 Pa
 *     011     |    x4        | 18 bit / 0.66 Pa
 *     100     |    x8        | 19 bit / 0.33 Pa
 * 101 110 111 |    x16       | 20 bit / 0.16 Pa
 * ---------------------------------------------
 */
#define BMP280_CTRL_MEAS_OSRS_P_SKIP		0x00
#define BMP280_CTRL_MEAS_OSRS_P_X1			0x04
#define BMP280_CTRL_MEAS_OSRS_P_X2			0x08
#define BMP280_CTRL_MEAS_OSRS_P_X4			0x0C
#define BMP280_CTRL_MEAS_OSRS_P_X8			0x10
#define BMP280_CTRL_MEAS_OSRS_P_X16			0x14

/* ------------------
 * mode[1:0] | Mode
 * ------------------
 *     00    | Sleep
 *   01 10   | Forced
 *     11    | Normal
 * ------------------
 */
#define BMP280_CTRL_MEAS_MODE_SLEEP			0x00
#define BMP280_CTRL_MEAS_MODE_FORCED		0x01
#define BMP280_CTRL_MEAS_MODE_NORMAL		0x03

/* --------------------------
 * Register 0xF5 "config"
 * --------------------------
 * t_sb[7:5] | t_standby [ms]
 * --------------------------
 *    000    |  0.5
 *    001    |  62.5
 *    010    |  125
 *    011    |  250
 *    100    |  500
 *    101    |  1000
 *    110    |  2000
 *    111    |  4000
 * --------------------------
 */
#define BMP280_CONFIG_T_SB_0_5				0x00
#define BMP280_CONFIG_T_SB_62_5				0x20
#define BMP280_CONFIG_T_SB_125				0x40
#define BMP280_CONFIG_T_SB_250				0x60
#define BMP280_CONFIG_T_SB_500				0x80
#define BMP280_CONFIG_T_SB_1000				0xA0
#define BMP280_CONFIG_T_SB_2000				0xC0
#define BMP280_CONFIG_T_SB_4000				0xE0


/* ---------------------------------------------------
 * filter[4:2] | Filter      | Samples to reach >=75 %
 *             | coefficient | of step response
 * ---------------------------------------------------
 *     000     | Filter off  |   1
 *     001     |     2       |   2
 *     010     |     4       |   5
 *     011     |     8       |   11
 *     100     |     16      |   22
 * ---------------------------------------------------
 */
#define BMP280_CONFIG_FILTER_OFF			0x00
#define BMP280_CONFIG_FILTER_COEFF_2		0x04
#define BMP280_CONFIG_FILTER_COEFF_4		0x08
#define BMP280_CONFIG_FILTER_COEFF_8		0x0C
#define BMP280_CONFIG_FILTER_COEFF_16		0x10

/* ------------------------------------------
 * spi3w_en[0] | Enables 3-wire SPI interface
 * ------------------------------------------
 *    0        | disable
 *    1        | enable
 * ------------------------------------------
 */
#define BMP280_CONFIG_SPI3W_OFF				0x00
#define BMP280_CONFIG_SPI3W_ON				0x01

typedef enum {
	 BMP280_Sleep  = 0x0,
	 BMP280_Forced = 0x1,
	 BMP280_Normal = 0x3
} BMP280_Mode_Type;

uint8_t BMP280_ReadRegister(uint8_t address);
void BMP280_ReadRegisters(uint8_t address, uint8_t *data, uint8_t size);
void BMP280_WriteRegister(uint8_t address, uint8_t data);
void BMP280_WriteRegisters(uint8_t address, uint8_t *data, uint8_t size);
uint8_t BMP280_GetStatus();
void BMP280_Init(BMP280_Mode_Type mode);
void BMP280_Reset();
void BMP280_SetRegisterCtrlMeas(uint8_t ctrl_meas);
void BMP280_SetRegisterConfig(uint8_t config);
double BMP280_Compensate_Temperature(int32_t adc_T);
double BMP280_Compensate_Pressure(int32_t adc_P);
double BMP280_GetTemperature();
double BMP280_GetPressure();
void BMP280_GetTemperatureAndPressure(double *temperature, double *pressure);
void BMP280_GetTemperatureAndPressureForced(double *temperature, double *pressure);

#endif /* BMP280_H_ */
