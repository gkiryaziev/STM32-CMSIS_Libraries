/*
 * I2C1
 * SCL: PB6
 * SDA: PB7
 *
 * I2C2
 * SCL: PB10
 * SDA: PB11
 */

#include "i2c.h"

#define PCLK1_FREQUENCY	(uint32_t)(SystemCoreClock / 2)	// PCLK1 frequency

void I2C1_Init(I2C_Mode_Type mode) {

	RCC->APB1ENR |= RCC_APB1ENR_I2C1EN;				// 1: I2C1 clock enabled
	RCC->APB2ENR |= RCC_APB2ENR_IOPBEN;				// 1: I/O port B clock enabled
	RCC->APB2ENR |= RCC_APB2ENR_AFIOEN;				// 1: Alternate Function I/O clock enabled

	// SCL1
	GPIOB->CRL |= GPIO_CRL_MODE6;					// 11: Output mode, max speed 50 MHz.
	GPIOB->CRL |= GPIO_CRL_CNF6;					// 11: Alternate function output Open-drain
	// SDA1
	GPIOB->CRL |= GPIO_CRL_MODE7;					// 11: Output mode, max speed 50 MHz.
	GPIOB->CRL |= GPIO_CRL_CNF7;					// 11: Alternate function output Open-drain

	// I2C
	I2C1->CR1 |= I2C_CR1_SWRST;						// 1: I2C Peripheral under reset state
	I2C1->CR1 = 0;									// Reset

	I2C1->CR2 |= (PCLK1_FREQUENCY / 1000000);		// FREQ[5:0]: Peripheral clock frequency

	if (mode == I2C_STANDARD) {
		I2C1->CCR = (PCLK1_FREQUENCY / (100000 * 2));					// CCR[11:0]: Clock control register in Fm/Sm mode (Master mode) 100kHz
		I2C1->TRISE = ((PCLK1_FREQUENCY / 1000000) + 1);				// TRISE[5:0]: Maximum rise time in Fm/Sm mode (Master mode)
	} else if (mode == I2C_FAST) {
		I2C1->CCR = (PCLK1_FREQUENCY / (400000 * 3));					// CCR[11:0]: Clock control register in Fm/Sm mode (Master mode) 400kHz
		I2C1->CCR |= I2C_CCR_FS;										// 1: Fm mode I2C (Fast mode - 400kHz)
		I2C1->TRISE = (((PCLK1_FREQUENCY / 1000000 * 300) / 1000) + 1);	// TRISE[5:0]: Maximum rise time in Fm/Sm mode (Master mode)
	}

	I2C1->CR1 |= I2C_CR1_PE;						// 1: Peripheral enable
}

void I2C1_Start() {

	//while ((I2C1->SR2 & I2C_SR2_BUSY));			// 1: Communication ongoing on the bus

	I2C1->CR1 |= I2C_CR1_START;						// 1: Repeated start generation

	while ( !(I2C1->SR1 & I2C_SR1_SB)  ||			// 0: No Start condition
			!(I2C1->SR2 & I2C_SR2_MSL) ||			// 0: Slave Mode
			!(I2C1->SR2 & I2C_SR2_BUSY)				// 0: No communication on the bus
	);
}

void I2C1_Stop() {

	I2C1->CR1 |= I2C_CR1_STOP;						// 1: Stop generation after the current byte transfer or after the current Start condition is sent.

	while((I2C1->SR1 & I2C_SR1_STOPF));				// 1: Stop condition detected
}

I2C_Status_Type I2C1_SendAddress(uint8_t address, I2C_Direction_Type direction) {

	uint32_t timeout = 1000000;

	address <<= 1;

	if (direction == I2C_TRANSMITTER) {
		address &= ~(1 << 0);						// Reset the address bit0 for write
		I2C1->DR = address;
		while ((!(I2C1->SR1 & I2C_SR1_ADDR) ||		// 0: No end of address transmission
				!(I2C1->SR2 & I2C_SR2_MSL)  ||		// 0: Slave Mode
				!(I2C1->SR2 & I2C_SR2_BUSY) ||		// 0: No communication on the bus
				!(I2C1->SR2 & I2C_SR2_TRA)) &&		// 0: Data bytes received
				--timeout
		);
	} else if (direction == I2C_RECEIVER) {
		address |= (1 << 0);						// Set the address bit0 for read
		I2C1->DR = address;
		while ((!(I2C1->SR1 & I2C_SR1_ADDR) ||		// 0: No end of address transmission
				!(I2C1->SR2 & I2C_SR2_MSL)  ||		// 0: Slave Mode
				!(I2C1->SR2 & I2C_SR2_BUSY))&&		// 0: No communication on the bus
				--timeout
		);
	}

	if (timeout <= 0) {
		return I2C_ERROR;
	}

	return I2C_OK;
}

void I2C1_SendData(uint8_t data) {

	I2C1->DR = data;

	while ( !(I2C1->SR1 & I2C_SR1_BTF)  ||			// 0: Data byte transfer not done
			!(I2C1->SR1 & I2C_SR1_TXE)  ||			// 0: Data register not empty
			!(I2C1->SR2 & I2C_SR2_MSL)  ||			// 0: Slave Mode
			!(I2C1->SR2 & I2C_SR2_BUSY) ||			// 0: No communication on the bus
			!(I2C1->SR2 & I2C_SR2_TRA)				// 0: Data bytes received
	);
}

uint8_t I2C1_ReceiveData(I2C_Acknowledge_Type acknowledge) {

	if (acknowledge == I2C_ACK) {
		I2C1->CR1 |= I2C_CR1_ACK;					// 1: Acknowledge returned after a byte is received (matched address or data)
	} else if (acknowledge == I2C_NACK) {
		I2C1->CR1 &= ~I2C_CR1_ACK;					// 0: No acknowledge returned
	}

	while ( !(I2C1->SR1 & I2C_SR1_RXNE) ||			// 0: Data register empty
			!(I2C1->SR2 & I2C_SR2_MSL)  ||			// 0: Slave Mode
			!(I2C1->SR2 & I2C_SR2_BUSY)				// 0: No communication on the bus
	);

	return (uint8_t)I2C1->DR;
}
