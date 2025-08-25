/*
 * PCF8574T.c
 *
 * Updated at: 31.05.2025
 * Created on: 25.05.2019
 *     Author: Admin
 */

#include "PCF8574T.h"

volatile uint8_t PCF8574T_Port = 0xFF;


// -----------------------------------------------
// PCF8574T_Init
// -----------------------------------------------
void PCF8574T_Init(void) {}

// -----------------------------------------------
// PCF8574T_GetStatus
// -----------------------------------------------
I2C_Status_Type PCF8574T_GetStatus(void) {

    I2C_Status_Type status = I2C1_Start();
    if (status != I2C_OK) return I2C_ERROR;

    status = I2C1_SendAddress(PCF8574T_ADDRESS, I2C_TRANSMITTER); 
    
    I2C1_Stop();
    
    return status;
}

// -----------------------------------------------
// PCF8574T_Read
// -----------------------------------------------
I2C_Status_Type PCF8574T_Read(uint8_t *data) {
    
    if (data == NULL) return I2C_ERROR;

    I2C_Status_Type status = I2C1_Start();
    if (status != I2C_OK) return I2C_ERROR;
    
    status = I2C1_SendAddress(PCF8574T_ADDRESS, I2C_RECEIVER);
    if (status != I2C_OK) {
        I2C1_Stop();
        return I2C_ERROR;
    }

    status = I2C1_ReceiveData(I2C_NACK, data); 
    if (status != I2C_OK) {
        return I2C_ERROR;
    }

    PCF8574T_Port = *data; 

    return I2C_OK;
}

// -----------------------------------------------
// PCF8574T_Write
// -----------------------------------------------
I2C_Status_Type PCF8574T_Write(uint8_t data) {

    I2C_Status_Type status = I2C1_Start();
    if (status != I2C_OK) {
        return I2C_ERROR;
    }
    
    status = I2C1_SendAddress(PCF8574T_ADDRESS, I2C_TRANSMITTER);
    if (status != I2C_OK) {
        I2C1_Stop();
        return I2C_ERROR;
    }

    status = I2C1_SendData(data);
    if (status != I2C_OK) {
        I2C1_Stop();
        return I2C_ERROR;
    }

    I2C1_Stop();
    
    PCF8574T_Port = data;

    return I2C_OK;
}

// -----------------------------------------------
// PCF8574T_ReadPin
// -----------------------------------------------
I2C_Status_Type PCF8574T_ReadPin(uint8_t pin, uint8_t *state) {
    
    if (state == NULL || pin > 7) return I2C_ERROR;

    uint8_t current_port_state;

    I2C_Status_Type status = PCF8574T_Read(&current_port_state);
    if (status != I2C_OK) return I2C_ERROR;

    *state = (PCF8574T_Port & (1 << pin)) ? 1 : 0;

    return I2C_OK;
}

// -----------------------------------------------
// PCF8574T_WritePin
// -----------------------------------------------
I2C_Status_Type PCF8574T_WritePin(uint8_t pin, uint8_t state) {
    
    if (pin > 7) return I2C_ERROR;

    if (state) PCF8574T_Port |= (1 << pin);
    else PCF8574T_Port &= ~(1 << pin);

    return PCF8574T_Write(PCF8574T_Port);
}

// -----------------------------------------------
// PCF8574T_GetPortState
// -----------------------------------------------
uint8_t PCF8574T_GetPortState(void) {
    return PCF8574T_Port;
}