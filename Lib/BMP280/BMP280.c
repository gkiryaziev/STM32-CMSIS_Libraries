/*
 * BMP280.c
 *
 *  Created on: 04.05.2019
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

uint8_t BMP280_ReadRegister(uint8_t address) {

	uint8_t data[1] = {0};

	BMP280_ReadRegisters(address, data, 1);

	return data[0];
}

void BMP280_ReadRegisters(uint8_t address, uint8_t *data, uint8_t size) {

	uint8_t count = 0;

	// I2C Send
	I2C1_Start();
	I2C1_SendAddress(BMP280_ADDRESS, I2C_TRANSMITTER);
	I2C1_SendData(address);

	// I2C Receive
	I2C1_Start();
	I2C1_SendAddress(BMP280_ADDRESS, I2C_RECEIVER);

	for (count = 0; count < size - 1; ++count ) {
		data[count] = I2C1_ReceiveData(I2C_ACK);
	}

	I2C1_Stop();
	data[count] = I2C1_ReceiveData(I2C_NACK);
}

void BMP280_WriteRegister(uint8_t address, uint8_t data) {

	uint8_t tempdata[1] = { data };

	BMP280_WriteRegisters(address, tempdata, 1);
}

void BMP280_WriteRegisters(uint8_t address, uint8_t *data, uint8_t size) {

	// I2C Send
	I2C1_Start();
	I2C1_SendAddress(BMP280_ADDRESS, I2C_TRANSMITTER);
	I2C1_SendData(address);

	for (int8_t i = 0; i < size; ++i) {
		I2C1_SendData(data[i]);
	}

	I2C1_Stop();
}

uint8_t BMP280_GetStatus() {

	// I2C Send
	I2C1_Start();
	if (I2C1_SendAddress(BMP280_ADDRESS, I2C_TRANSMITTER) == I2C_ERROR) return 0;
	I2C1_SendData(BMP280_REGISTER_ID);

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

void BMP280_Init(BMP280_Mode_Type mode) {

	uint8_t data[2] = {0};

	// Temperature compensation related values, (LSB first)
	BMP280_ReadRegisters(BMP280_DIG_T1_LSB, data, 2);
	BMP280_dig_T1 = data[1] << 8 | data[0];

	BMP280_ReadRegisters(BMP280_DIG_T2_LSB, data, 2);
	BMP280_dig_T2 = data[1] << 8 | data[0];

	BMP280_ReadRegisters(BMP280_DIG_T3_LSB, data, 2);
	BMP280_dig_T3 = data[1] << 8 | data[0];

	// Pressure compensation related values, (LSB first)
	BMP280_ReadRegisters(BMP280_DIG_P1_LSB, data, 2);
	BMP280_dig_P1 = data[1] << 8 | data[0];

	BMP280_ReadRegisters(BMP280_DIG_P2_LSB, data, 2);
	BMP280_dig_P2 = data[1] << 8 | data[0];

	BMP280_ReadRegisters(BMP280_DIG_P3_LSB, data, 2);
	BMP280_dig_P3 = data[1] << 8 | data[0];

	BMP280_ReadRegisters(BMP280_DIG_P4_LSB, data, 2);
	BMP280_dig_P4 = data[1] << 8 | data[0];

	BMP280_ReadRegisters(BMP280_DIG_P5_LSB, data, 2);
	BMP280_dig_P5 = data[1] << 8 | data[0];

	BMP280_ReadRegisters(BMP280_DIG_P6_LSB, data, 2);
	BMP280_dig_P6 = data[1] << 8 | data[0];

	BMP280_ReadRegisters(BMP280_DIG_P7_LSB, data, 2);
	BMP280_dig_P7 = data[1] << 8 | data[0];

	BMP280_ReadRegisters(BMP280_DIG_P8_LSB, data, 2);
	BMP280_dig_P8 = data[1] << 8 | data[0];

	BMP280_ReadRegisters(BMP280_DIG_P9_LSB, data, 2);
	BMP280_dig_P9 = data[1] << 8 | data[0];

	// Soft reset
	// BMP280_Reset();

	if (mode == BMP280_Normal) {
		BMP280_SetRegisterConfig(BMP280_PRESET_CONFIG);
		BMP280_SetRegisterCtrlMeas(BMP280_PRESET_CTRL_MEAS_NORMAL);
		DWT_delay_ms(10);
	}
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

// --------------------------------------------------
// BMP280_Compensate_Temperature
// --------------------------------------------------
double BMP280_Compensate_Temperature(int32_t adc_T)
{
	int32_t var1, var2, t;

	var1 = ((((adc_T >> 3) - ((int32_t)BMP280_dig_T1 << 1))) * ((int32_t)BMP280_dig_T2)) >> 11;
	var2 = (((((adc_T >> 4) - (int32_t)BMP280_dig_T1) * ((adc_T >> 4) - (int32_t)BMP280_dig_T1)) >> 12) *
			(int32_t)BMP280_dig_T3) >> 14;
	BMP280_t_fine = var1 + var2;
	t = (BMP280_t_fine * 5 + 128) >> 8;

	return (double)t / 100.0;
}

// --------------------------------------------------
// BMP280_Compensate_Pressure (Pa)
// --------------------------------------------------
double BMP280_Compensate_Pressure(int32_t adc_P)
{
	int64_t var1, var2, p;

	var1 = (int64_t)BMP280_t_fine - 128000;
	var2 = var1 * var1 * (int64_t)BMP280_dig_P6;
	var2 = var2 + ((var1 * (int64_t)BMP280_dig_P5) << 17);
	var2 = var2 + ((int64_t)BMP280_dig_P4 << 35);
	var1 = ((var1 * var1 * (int64_t)BMP280_dig_P3) >> 8) + ((var1 * (int64_t)BMP280_dig_P2) << 12);
	var1 = ((((int64_t)1 << 47) + var1)) * (int64_t)BMP280_dig_P1 >> 33;

	if (var1 == 0)
	{
		return 0; // avoid exception caused by division by zero
	}

	p = 1048576 - adc_P;
	p = (((p << 31) - var2) * 3125) / var1;
	var1 = ((int64_t)BMP280_dig_P9 * (p >> 13) * (p >> 13)) >> 25;
	var2 = ((int64_t)BMP280_dig_P8 * p) >> 19;
	p = ((p + var1 + var2) >> 8) + ((int64_t)BMP280_dig_P7 << 4);

	return (double)p / 256.0;
}

double BMP280_GetTemperature() {

	int32_t adc_T;
	uint8_t data[3] = {0};

	BMP280_ReadRegisters(BMP280_REGISTER_TEMPERATURE_MSB, data, 3);

	adc_T = (int32_t)((data[0] << 12) | (data[1] << 4) | (data[2] >> 4));

	return BMP280_Compensate_Temperature(adc_T);
}

double BMP280_GetPressure() {

	int32_t adc_P;
	uint8_t data[3] = {0};

	BMP280_ReadRegisters(BMP280_REGISTER_PRESSURE_MSB, data, 3);

	adc_P = (int32_t)((data[0] << 12) | (data[1] << 4) | (data[2] >> 4));

	return BMP280_Compensate_Pressure(adc_P);
}

void BMP280_GetTemperatureAndPressure(double *temperature, double *pressure) {

	int32_t adc_T, adc_P;
	uint8_t data[6] = {0};

	BMP280_ReadRegisters(BMP280_REGISTER_PRESSURE_MSB, data, 6);

	adc_P = (int32_t)((data[0] << 12) | (data[1] << 4) | (data[2] >> 4));
	adc_T = (int32_t)((data[3] << 12) | (data[4] << 4) | (data[5] >> 4));

	*temperature = BMP280_Compensate_Temperature(adc_T);
	*pressure = BMP280_Compensate_Pressure(adc_P);
}

void BMP280_GetTemperatureAndPressureForced(double *temperature, double *pressure) {

	// 20bit, Forced mode, 125ms, filter 2
	BMP280_SetRegisterConfig(BMP280_PRESET_CONFIG);
	BMP280_SetRegisterCtrlMeas(BMP280_PRESET_CTRL_MEAS_FORCED);

	DWT_delay_ms(10);

	*temperature = BMP280_GetTemperature();
	*pressure = BMP280_GetPressure();
}
