## STM32 (STM32F103C8T6) CMSIS Libraries

IDE - STM32CubeIDE 1.7.0

#### System (Inc/Src)
- **RCC**
- **GPIO**
- **Delay**
- **USART**
- **I2C**
- **SPI**
- **RTC** (Lib)
- **PWM** (Lib)
- **EXTI** (Lib)

#### Other (Lib)
- **74HC595** (SPI/BitBang)
- **MAX6675** (SPI/BitBang)
- **MPU9255** (SPI)
- **ADS1115** (I2C)
- **BMP280** (I2C)
- **PCF8574T** (I2C)
- **EEPROM/24C64** (I2C)
- **LSM303DLHC** (I2C)
- **LCD1602** (8-bit/4-bit/PCF8574T)
- **LED_DISPLAY** (74HC595)
- **HCSR04**
- **DHT11**
- **Rotary Encoder (polling/debounce)**

#### Information
```c
// non blocking delay example
Delay_TypeDef d1, d2, usart1_d1;

char buffer[USART1_BUFFER_SIZE] = {0};

if (DWT_nb_timeout(&d1)) {
	GPIO_WritePin(GPIOB, 12 , state1); state1 = !state1;
	DWT_nb_delay_ms(&d1, 300);
}

if (DWT_nb_timeout(&d2)) {
	GPIO_WritePin(GPIOB, 14 , state2); state2 = !state2;
	DWT_nb_delay_ms(&d2, 700);
}

// non blocking delay, USART1
if (DWT_nb_timeout(&usart1_d1)) {
	if (USART1_ReadString(buffer))
		USART1_SendString(buffer);
	DWT_nb_delay_ms(&usart1_d1, 50);
}
```

*All libraries are under development and are provided as is.*
