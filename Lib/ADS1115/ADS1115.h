#ifndef ADS1115_H_
#define ADS1115_H_

#include "i2c.h"
#include "delay.h"

/* -----------------------------------------------------------------------------------
 * Config Register Field Descriptions  [0:7] CRL, [8:15] CRH
 * -----------------------------------------------------------------------------------
 * 15    | OS             | Operational status or single-shot conversion start
 *       |                | When writing:
 *       |                | 0   : No effect
 *       |                | 1   : Start a single conversion (when in power-down state)
 *       |                | When reading:
 *       |                | 0   : Device is currently performing a conversion
 *       |                | 1   : Device is not currently performing a conversion
 * -----------------------------------------------------------------------------------
 * 14:12 | MUX [2:0]      | Input multiplexer configuration
 *       |                | 000 : AINP = AIN0 and AINN = AIN1 (default)
 *       |                | 001 : AINP = AIN0 and AINN = AIN3
 *       |                | 010 : AINP = AIN1 and AINN = AIN3
 *       |                | 011 : AINP = AIN2 and AINN = AIN3
 *       |                | 100 : AINP = AIN0 and AINN = GND
 *       |                | 101 : AINP = AIN1 and AINN = GND
 *       |                | 110 : AINP = AIN2 and AINN = GND
 *       |                | 111 : AINP = AIN3 and AINN = GND
 * -----------------------------------------------------------------------------------
 * 11:9  | PGA [2:0]      | Programmable gain amplifier configuration
 *       |                | 000 : FSR = В±6.144 V
 *       |                | 001 : FSR = В±4.096 V
 *       |                | 010 : FSR = В±2.048 V (default)
 *       |                | 011 : FSR = В±1.024 V
 *       |                | 100 : FSR = В±0.512 V
 *       |                | 101 : FSR = В±0.256 V
 *       |                | 110 : FSR = В±0.256 V
 *       |                | 111 : FSR = В±0.256 V
 * -----------------------------------------------------------------------------------
 * 8     | MODE           | Device operating mode
 *       |                | 0   : Continuous-conversion mode
 *       |                | 1   : Single-shot mode or power-down state (default)
 * -----------------------------------------------------------------------------------
 * 7:5   | DR [2:0]       | Data rate
 *       |                | 000 : 8 SPS
 *       |                | 001 : 16 SPS
 *       |                | 010 : 32 SPS
 *       |                | 011 : 64 SPS
 *       |                | 100 : 128 SPS (default)
 *       |                | 101 : 250 SPS
 *       |                | 110 : 475 SPS
 *       |                | 111 : 860 SPS
 * -----------------------------------------------------------------------------------
 * 4     | COMP_MODE      | Comparator mode
 *       |                | 0   : Traditional comparator (default)
 *       |                | 1   : Window comparator
 * -----------------------------------------------------------------------------------
 * 3     | COMP_POL       | Comparator polarity
 *       |                | 0   : Active low (default)
 *       |                | 1   : Active high
 * -----------------------------------------------------------------------------------
 * 2     | COMP_LAT       | Latching comparator
 *       |                | 0   : Nonlatching comparator. (default)
 *       |                | 1   : Latching comparator.
 * -----------------------------------------------------------------------------------
 * 1:0   | COMP_QUE [1:0] | Comparator queue and disable
 *       |                | 00  : Assert after one conversion
 *       |                | 01  : Assert after two conversions
 *       |                | 10  : Assert after four conversions
 *       |                | 11  : Disable comparator and set ALERT/RDY pin to high-impedance (default)
 * -----------------------------------------------------------------------------------
 *
 * ----------------------------------------------------
 * Table 3. Full-Scale Range and Corresponding LSB Size
 * ----------------------------------------------------
 *   FSR       | LSB SIZE
 * ----------------------------------------------------
 *   ±6.144 V  | 187.5  μV
 *   ±4.096 V  | 125    μV
 *   ±2.048 V  | 62.5   μV
 *   ±1.024 V  | 31.25  μV
 *   ±0.512 V  | 15.625 μV
 *   ±0.256 V  | 7.8125 μV
 * ----------------------------------------------------
 */

#define ADS1115_ADDRESS				0x48

#define ADS1115_REG_CONVERSION		0x00
#define ADS1115_REG_CONFIG			0x01
#define ADS1115_REG_LO_THRESH		0x02
#define ADS1115_REG_HI_THRESH		0x03

#define ADS1115_OS_Pos				(15U)
#define ADS1115_OS_Msk				(0x1U << ADS1115_OS_Pos)
#define ADS1115_OS					ADS1115_OS_Msk

#define ADS1115_MUX_Pos				(12U)
#define ADS1115_MUX_Msk				(0x7U << ADS1115_MUX_Pos)
#define ADS1115_MUX					ADS1115_MUX_Msk
#define ADS1115_MUX_0				(0x1U << ADS1115_MUX_Pos)
#define ADS1115_MUX_1				(0x2U << ADS1115_MUX_Pos)
#define ADS1115_MUX_2				(0x4U << ADS1115_MUX_Pos)

#define ADS1115_PGA_Pos				(9U)
#define ADS1115_PGA_Msk				(0x7U << ADS1115_PGA_Pos)
#define ADS1115_PGA					ADS1115_PGA_Msk
#define ADS1115_PGA_0				(0x1U << ADS1115_PGA_Pos)
#define ADS1115_PGA_1				(0x2U << ADS1115_PGA_Pos)
#define ADS1115_PGA_2				(0x4U << ADS1115_PGA_Pos)

#define ADS1115_MODE_Pos			(8U)
#define ADS1115_MODE_Msk			(0x1U << ADS1115_MODE_Pos)
#define ADS1115_MODE				ADS1115_MODE_Msk

#define ADS1115_DR_Pos				(5U)
#define ADS1115_DR_Msk				(0x7U << ADS1115_DR_Pos)
#define ADS1115_DR					ADS1115_DR_Msk
#define ADS1115_DR_0				(0x1U << ADS1115_DR_Pos)
#define ADS1115_DR_1				(0x2U << ADS1115_DR_Pos)
#define ADS1115_DR_2				(0x4U << ADS1115_DR_Pos)

#define ADS1115_COMP_MODE_Pos		(4U)
#define ADS1115_COMP_MODE_Msk		(0x1U << ADS1115_COMP_MODE_Pos)
#define ADS1115_COMP_MODE			ADS1115_COMP_MODE_Msk

#define ADS1115_COMP_POL_Pos		(3U)
#define ADS1115_COMP_POL_Msk		(0x1U << ADS1115_COMP_POL_Pos)
#define ADS1115_COMP_POL			ADS1115_COMP_POL_Msk

#define ADS1115_COMP_LAT_Pos		(2U)
#define ADS1115_COMP_LAT_Msk		(0x1U << ADS1115_COMP_LAT_Pos)
#define ADS1115_COMP_LAT			ADS1115_COMP_LAT_Msk

#define ADS1115_COMP_QUE_Pos		(0U)
#define ADS1115_COMP_QUE_Msk		(0x3U << ADS1115_COMP_QUE_Pos)
#define ADS1115_COMP_QUE			ADS1115_COMP_QUE_Msk
#define ADS1115_COMP_QUE_0			(0x1U << ADS1115_COMP_QUE_Pos)
#define ADS1115_COMP_QUE_1			(0x2U << ADS1115_COMP_QUE_Pos)

void ADS1115_ReadRegisters(uint8_t address, uint8_t *data, uint8_t size);
void ADS1115_WriteRegisters(uint8_t address, uint8_t *data, uint8_t size);
uint8_t ADS1115_GetStatus(void);
uint16_t ADS1115_ReadADC(uint8_t channel);
float ADS1115_Read(uint8_t channel);

#endif /* ADS1115_H_ */
