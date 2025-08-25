
# 74HC595 8-Bit Shift Register Driver

This library provides low-level functions to control `74HC595` 8-bit "Serial-In, Parallel-Out" shift registers. It allows you to expand the number of GPIO output pins on your microcontroller using just three control lines.

The library supports two methods for communication:
1.  **Hardware SPI (Recommended)**: Uses the MCU's dedicated SPI peripheral for fast and efficient data transfer.
2.  **Software Bit-Banging**: Manually toggles GPIO pins to simulate the protocol, offering flexibility if SPI pins are in use.

The driver also includes a function to control multiple, daisy-chained shift registers.

## Hardware Setup

### Single 74HC595 Chip
Connect the chip to your STM32 as follows.
*   **VCC**: to +3.3V or +5V (depending on your logic level)
*   **GND**: to GND
*   **~OE (Output Enable)**: to GND (to permanently enable the outputs)
*   **~SRCLR (Shift Register Clear)**: to VCC (to prevent accidental clearing)
*   **SER (Serial Data In)**: to MCU's `DATA` pin (`PB14` or `PA7` for SPI MOSI)
*   **SRCLK (Shift Register Clock)**: to MCU's `CLOCK` pin (`PB13` or `PA5` for SPI SCK)
*   **RCLK (Register Clock / Latch)**: to MCU's `LATCH` pin (`PB12`)

### Daisy-Chaining Multiple Chips
To control more than 8 outputs, you can chain multiple registers together.
*   Connect the `QH'` (Serial Data Out) pin of the first `74HC595` to the `SER` pin of the second one.
*   Connect the `CLOCK` and `LATCH` lines to all chips in parallel.

## How it Works: The Latch Pin
Data is shifted into the register one bit at a time on the rising edge of the **CLOCK** pin. However, the parallel outputs (`QA` - `QH`) do not change immediately. The outputs are only updated with the new data when the **LATCH** pin is pulsed (from LOW to HIGH and back to LOW). This library's send functions handle the latching automatically after the data has been shifted in.

## Usage Guide

### Step 1: Initialization (Choose ONE Method)

**A) Hardware SPI (Recommended)**
```c
// In your initialization code
SPI1_Init();        // Initialize the SPI1 peripheral
HC595_SPI_Init();   // Initialize the LATCH pin
```

**B) Software Bit-Bang**
```c
// In your initialization code
HC595_BitBang_Init(); // Initializes DATA, CLOCK, and LATCH pins as GPIO outputs
```

### Step 2: Sending Data

#### A) Controlling a Single Register
Use `HC595_SPI_SendByte()` or `HC595_BitBang_SendByte()` to send 8 bits of data.

**Example 1: Controlling 8 LEDs with a Single 74HC595**
This example will create a binary counting effect on 8 LEDs connected to the `QA`-`QH` outputs.
```c
#include "main.h"
#include "74HC595.h"

int main(void) {
    // System Init
    RCC_Init();
    DWT_Init();
    
    // 74HC595 Init (using SPI)
    SPI1_Init();
    HC595_SPI_Init();

    // Loop forever, sending a binary count to the 8 LEDs
    for (uint8_t i = 0; ; ++i) {
        HC595_SPI_SendByte(i);
        DWT_delay_ms(50);
        if (i == 255) i = 0; // Reset counter
    }
}
```

#### B) Controlling Daisy-Chained Registers
Use `HC595_SPI_SendBuffer()` to control multiple registers in a chain.

**Important:** The first byte sent in the buffer ends up in the **last** register in the chain. The last byte sent ends up in the first register (the one closest to the MCU).

**Example 2: Controlling 16 LEDs with Two Daisy-Chained Registers**
This example sends one pattern to the first register and another to the second.
```c
#include "main.h"
#include "74HC595.h"

int main(void) {
    // System and 74HC595 Init (using SPI)
    RCC_Init();
    DWT_Init();
    SPI1_Init();
    HC595_SPI_Init();

    // A buffer for two registers (16 bits total)
    uint8_t data_buffer[2];

    while(1) {
        // Pattern 1: First 4 LEDs on, last 4 off for both chips
        data_buffer[0] = 0x0F; // This data goes to the SECOND chip in the chain
        data_buffer[1] = 0x0F; // This data goes to the FIRST chip in the chain
        HC595_SPI_SendBuffer(data_buffer, 2);
        DWT_delay_ms(500);

        // Pattern 2: Last 4 LEDs on, first 4 off for both chips
        data_buffer[0] = 0xF0; // Data for the SECOND chip
        data_buffer[1] = 0xF0; // Data for the FIRST chip
        HC595_SPI_SendBuffer(data_buffer, 2);
        DWT_delay_ms(500);
    }
}
```