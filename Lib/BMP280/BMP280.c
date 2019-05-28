/*
 * BMP280.c
 *
 *  Created on: 4 мая 2019 г.
 *      Author: Admin
 */

#include "BMP280.h"

uint16_t BMP280_dig_T1;
int16_t BMP280_dig_T2;
int16_t BMP280_dig_T3;
uint16_t BMP280_dig_P1;
int16_t BMP280_dig_P2;
int16_t BMP280_dig_P3;
int16_t BMP280_dig_P4;
int16_t BMP280_dig_P5;
int16_t BMP280_dig_P6;
int16_t BMP280_dig_P7;
int16_t BMP280_dig_P8;
int16_t BMP280_dig_P9;
int32_t BMP280_t_fine;

uint8_t BMP280_GetStatus() {

	// I2C Send
	I2C1_Start();
	if (I2C1_SendAddress(BMP280_ADDRESS, I2C_TRANSMITTER) == I2C_ERROR) return 0;
	I2C1_SendData(BMP280_REGISTER_ID);
	I2C1_Stop();

	DWT_delay_ms(1);

	// I2C Receive
	I2C1_Start();
	if (I2C1_SendAddress(BMP280_ADDRESS, I2C_RECEIVER) == I2C_ERROR) return 0;
	I2C1_Stop();
	uint8_t id = I2C1_ReceiveData(I2C_NACK);

	if(id != 0x58) {
		return 0;
	}

	return 1;
}

uint8_t BMP280_ReadRegister(uint8_t address) {

	// I2C Send
	I2C1_Start();
	I2C1_SendAddress(BMP280_ADDRESS, I2C_TRANSMITTER);
	I2C1_SendData(address);
	I2C1_Stop();

	DWT_delay_ms(1);

	// I2C Receive
	I2C1_Start();
	I2C1_SendAddress(BMP280_ADDRESS, I2C_RECEIVER);
	I2C1_Stop();

	return I2C1_ReceiveData(I2C_NACK);
}

void BMP280_WriteRegister(uint8_t address, uint8_t data) {

	// I2C Send
	I2C1_Start();
	I2C1_SendAddress(BMP280_ADDRESS, I2C_TRANSMITTER);
	I2C1_SendData(address);
	I2C1_SendData(data);
	I2C1_Stop();
}

void BMP280_Init() {

	uint8_t lsb, msb;

	// Temperature compensation related values
	lsb = BMP280_ReadRegister(BMP280_DIG_T1_LSB);
	msb = BMP280_ReadRegister(BMP280_DIG_T1_MSB);
	BMP280_dig_T1 = msb << 8 | lsb;

	lsb = BMP280_ReadRegister(BMP280_DIG_T2_LSB);
	msb = BMP280_ReadRegister(BMP280_DIG_T2_MSB);
	BMP280_dig_T2 = msb << 8 | lsb;

	lsb = BMP280_ReadRegister(BMP280_DIG_T3_LSB);
	msb = BMP280_ReadRegister(BMP280_DIG_T3_MSB);
	BMP280_dig_T3 = msb << 8 | lsb;

	// Pressure compensation related values
	lsb = BMP280_ReadRegister(BMP280_DIG_P1_LSB);
	msb = BMP280_ReadRegister(BMP280_DIG_P1_MSB);
	BMP280_dig_P1 = msb << 8 | lsb;

	lsb = BMP280_ReadRegister(BMP280_DIG_P2_LSB);
	msb = BMP280_ReadRegister(BMP280_DIG_P2_MSB);
	BMP280_dig_P2 = msb << 8 | lsb;

	lsb = BMP280_ReadRegister(BMP280_DIG_P3_LSB);
	msb = BMP280_ReadRegister(BMP280_DIG_P3_MSB);
	BMP280_dig_P3 = msb << 8 | lsb;

	lsb = BMP280_ReadRegister(BMP280_DIG_P4_LSB);
	msb = BMP280_ReadRegister(BMP280_DIG_P4_MSB);
	BMP280_dig_P4 = msb << 8 | lsb;

	lsb = BMP280_ReadRegister(BMP280_DIG_P5_LSB);
	msb = BMP280_ReadRegister(BMP280_DIG_P5_MSB);
	BMP280_dig_P5 = msb << 8 | lsb;

	lsb = BMP280_ReadRegister(BMP280_DIG_P6_LSB);
	msb = BMP280_ReadRegister(BMP280_DIG_P6_MSB);
	BMP280_dig_P6 = msb << 8 | lsb;

	lsb = BMP280_ReadRegister(BMP280_DIG_P7_LSB);
	msb = BMP280_ReadRegister(BMP280_DIG_P7_MSB);
	BMP280_dig_P7 = msb << 8 | lsb;

	lsb = BMP280_ReadRegister(BMP280_DIG_P8_LSB);
	msb = BMP280_ReadRegister(BMP280_DIG_P8_MSB);
	BMP280_dig_P8 = msb << 8 | lsb;

	lsb = BMP280_ReadRegister(BMP280_DIG_P9_LSB);
	msb = BMP280_ReadRegister(BMP280_DIG_P9_MSB);
	BMP280_dig_P9 = msb << 8 | lsb;

	// Soft reset
	BMP280_Reset();

	// BMP280_SetRegisterConfig(BMP280_PRESET_CONFIG_NORMAL);
	// BMP280_SetRegisterCtrlMeas(BMP280_PRESET_CTRL_MEAS_NORMAL);

	// DWT_delay_ms(10);
}

void BMP280_Reset() {
	BMP280_WriteRegister(BMP280_REGISTER_RESET, BMP280_RESET_VALUE);
}

void BMP280_SetRegisterCtrlMeas(uint8_t ctrl_meas) {
	BMP280_WriteRegister(BMP280_REGISTER_CTRL_MEAS, ctrl_meas);
}

void BMP280_SetRegisterConfig(uint8_t config) {
	BMP280_WriteRegister(BMP280_REGISTER_CONFIG, config);
}

double BMP280_Compensate_Temperature_Double(int32_t adc_T) {

	double var1, var2;

	var1 = (((double) adc_T) / 16384.0 - ((double) BMP280_dig_T1) / 1024.0)
			* ((double) BMP280_dig_T2);
	var2 = ((((double) adc_T) / 131072.0 - ((double) BMP280_dig_T1) / 8192.0)
			* (((double) adc_T) / 131072.0 - ((double) BMP280_dig_T1) / 8192.0))
					* ((double) BMP280_dig_T3);
	BMP280_t_fine = (int32_t) (var1 + var2);

	return (var1 + var2) / 5120.0;
}

double BMP280_Compensate_Pressure_Double(int32_t adc_P) {

	double var1, var2, pressure;

    var1 = ((double)BMP280_t_fine / 2.0) - 64000.0;
    var2 = var1 * var1 * ((double)BMP280_dig_P6) / 32768.0;
    var2 = var2 + var1 * ((double)BMP280_dig_P5) * 2.0;
    var2 = (var2 / 4.0) + (((double)BMP280_dig_P4) * 65536.0);
    var1 = (((double)BMP280_dig_P3) * var1 * var1 / 524288.0
            + ((double)BMP280_dig_P2) * var1) / 524288.0;
    var1 = (1.0 + var1 / 32768.0) * ((double)BMP280_dig_P1);

    if (var1 == 0.0) {
        return 0; // avoid exception caused by division by zero
    }

    pressure = 1048576.0 - (double)adc_P;
    pressure = (pressure - (var2 / 4096.0)) * 6250.0 / var1;
    var1 = ((double)BMP280_dig_P9) * pressure * pressure / 2147483648.0;
    var2 = pressure * ((double)BMP280_dig_P8) / 32768.0;
    pressure = pressure + (var1 + var2 + ((double)BMP280_dig_P7)) / 16.0;

    return pressure;
}

double BMP280_GetTemperatureNormal() {

	uint8_t lsb, msb, xlsb;
	int32_t adc_T;

	xlsb = BMP280_ReadRegister(BMP280_REGISTER_TEMPERATURE_XLSB);
	lsb = BMP280_ReadRegister(BMP280_REGISTER_TEMPERATURE_LSB);
	msb = BMP280_ReadRegister(BMP280_REGISTER_TEMPERATURE_MSB);

	adc_T = (msb << 12) | (lsb << 4) | (xlsb >> 4);

	return BMP280_Compensate_Temperature_Double(adc_T);
}

double BMP280_GetPressureNormal() {

	uint8_t lsb, msb, xlsb;
	int32_t adc_P;

	xlsb = BMP280_ReadRegister(BMP280_REGISTER_PRESSURE_XLSB);
	lsb = BMP280_ReadRegister(BMP280_REGISTER_PRESSURE_LSB);
	msb = BMP280_ReadRegister(BMP280_REGISTER_PRESSURE_MSB);

	adc_P = (msb << 12) | (lsb << 4) | (xlsb >> 4);

	return BMP280_Compensate_Pressure_Double(adc_P);
}

void BMP280_GetTemperatureAndPressureNormal(double *temperature, double *pressure) {

	*temperature = BMP280_GetTemperatureNormal();
	*pressure = BMP280_GetPressureNormal();
}

void BMP280_GetTemperatureAndPressureForced(double *temperature, double *pressure) {

	// 20bit, Forced mode, 125ms, filter 2
	BMP280_SetRegisterConfig(BMP280_PRESET_CONFIG_FORCED);
	BMP280_SetRegisterCtrlMeas(BMP280_PRESET_CTRL_MEAS_FORCED);

	DWT_delay_ms(10);

	*temperature = BMP280_GetTemperatureNormal();
	*pressure = BMP280_GetPressureNormal();
}
