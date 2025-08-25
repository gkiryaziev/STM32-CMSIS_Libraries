
# STM32F103C8T6 Bare-Metal Peripheral Libraries

This repository contains a set of simple, bare-metal peripheral drivers for the STM32F103C8T6 microcontroller (popularly used in "Blue Pill" boards). The libraries are written using direct register access (CMSIS) for performance and educational purposes, providing a lightweight alternative to HAL.

### *All libraries are under development and are provided as is.*

## Features
*   **RCC**: System Clock Configuration (sets up 72MHz from 8MHz external crystal).
*   **GPIO**: General Purpose I/O control (input, output, alternate functions).
*   **DWT Delay**: High-precision microsecond and millisecond delays using the Core DWT cycle counter, without consuming a hardware timer.
*   **USART**: Interrupt-driven serial communication with a circular buffer for non-blocking reception.
*   **I2C**: Master mode communication functions.
*   **SPI**: Master mode communication functions with manual chip select.
*   **[PCF8574T I/O Expander](lib/PCF8574T/README.md)**: A driver for the popular 8-bit I2C I/O expander.
*   **[LCD1602 Character Display](lib/LCD1602/README.md)**: Drivers for HD44780-based displays via I2C, 4-bit, and 8-bit parallel modes.
*   **[74HC595 Shift Register](lib/74HC595/README.md)**: Low-level driver for expanding GPIO outputs, with support for hardware SPI and software bit-banging.
*   **[Multiplexed 7-Segment Display](lib/LED_Display/README.md)**: High-level, interrupt-driven driver for a 2-digit display controlled by a 74HC595 shift register.

## Getting Started

A typical application requires initializing the system clock first, followed by the delay mechanism, and then the specific peripherals you intend to use.

```c
#include "main.h" // A master header that includes all library headers

int main(void) {
    // 1. Configure the system clock.
    // This sets the MCU to its maximum speed (72MHz) using an 8MHz external crystal.
    // It is the most crucial first step, as all peripheral clocks depend on it.
    RCC_Init();

    // 2. Initialize the DWT unit for accurate delays.
    // The delay functions rely on the DWT's cycle counter and the SystemCoreClock frequency.
    DWT_Init();

    // 3. Initialize the peripherals you will use.
    GPIO_Init();   // A helper that configures the onboard LED (PC13).
    USART1_Init(); // Configures pins and enables USART1 with interrupts.
    I2C1_Init(I2C_STANDARD); // Configures I2C1 for 100kHz communication.
    SPI1_Init();   // Configures SPI1 pins and peripheral.
    // ... initialize other peripherals as needed ...

    // 4. Enter the main application loop.
    while(1) {
        // Your non-blocking application code goes here.
    }
}
```

## Usage Examples

### GPIO Control

The GPIO library allows you to configure pins as inputs or outputs and control their state.

**Example: Blinking the onboard LED (PC13) on a Blue Pill board.**

```c
// This code goes inside your main() function, after RCC_Init() and DWT_Init().

// 1. Enable the peripheral clock for GPIOC.
// Every peripheral needs its clock enabled before it can be configured.
GPIO_Enable(GPIOC);

// 2. Configure Pin 13 as a 50MHz Push-Pull Output.
// MODE = Output 50MHz, CNF = General Purpose Push-Pull.
GPIO_SetMode_Output_50MHz_PP(GPIOC, 13);

// This is the main application loop.
while(1) {
    // Set PC13 High. On a Blue Pill, the LED is connected to VCC,
    // so a high signal turns it OFF.
    GPIO_SetPin(GPIOC, 13);
    DWT_delay_ms(500); // Wait for half a second.

    // Set PC13 Low. This sinks current and turns the LED ON.
    GPIO_ResetPin(GPIOC, 13);
    DWT_delay_ms(500); // Wait for another half a second.
}
```

### DWT Delays

The DWT unit provides a CPU cycle counter, which is perfect for creating delays without using a hardware timer like SysTick.

#### Blocking Delays
These functions halt the CPU execution for a specified duration. They are simple to use and are great for initialization sequences, but should be avoided in the main loop of complex applications.

```c
// Pause execution for 200 milliseconds.
DWT_delay_ms(200);

// Pause execution for 750 microseconds.
DWT_delay_us(750);
```

#### Non-Blocking Delays
This method is essential for writing responsive applications that can handle multiple tasks. It checks if a time interval has passed without halting the CPU.

```c
// --- In the global scope, define timer state variables ---
Delay_TypeDef blinker_timer = {0};
uint8_t led_state = 0;

// --- In your main() function, inside the while(1) loop ---

// This code will not block other tasks from running.
if (DWT_nb_timeout(&blinker_timer)) {
    // This block executes only when the timer expires (e.g., every 300ms).

    // Toggle the LED state using the generic WritePin function
    GPIO_WritePin(GPIOC, 13, led_state);
    led_state = !led_state; // Invert the state for the next blink

    // IMPORTANT: Reset the timer for the next 300ms interval.
    DWT_nb_delay_ms(&blinker_timer, 300);
}

// You can run other tasks here. They will execute on every loop iteration.
// Check_USART_for_data();
// Read_sensor_data();
```

### USART Communication

The USART driver uses interrupts and a circular buffer to receive data in the background. This means your main loop can check for new data without having to wait for it.

**Example: An "echo" server that reads a line of text and sends it back.**

```c
// In your main() function, after all initializations
USART1_SendString("System Ready. Send a line of text ending with Enter.\n");

char received_line[USART1_BUFFER_SIZE];

// In your main() function, inside the while(1) loop
uint16_t bytes_read = USART1_ReadString(received_line);

// Check if a complete line was received (terminated by '\n' or '\r').
if (bytes_read > 0) {
    // A line is available in 'received_line'. Echo it back.
    USART1_SendString("You sent: ");
    USART1_SendString(received_line);
    USART1_SendString("\n"); // Add a newline for clean formatting in the terminal.
}

// Other tasks can run here...
```

### I2C Master Communication

The I2C library provides the fundamental functions to act as an I2C master. You must orchestrate these functions in the correct sequence to communicate with a slave device.

#### Example 1: Writing Data (Sensor Configuration)

Here, we write a single byte `0x01` to a configuration register `0x0F` of a hypothetical sensor at I2C address `0x3A`.

```c
#define SENSOR_I2C_ADDR   0x3A
#define SENSOR_CONFIG_REG 0x0F
#define SENSOR_CONFIG_VAL 0x01

void Configure_Sensor(void) {
    // Ensure I2C1 is initialized, e.g., I2C1_Init(I2C_STANDARD);

    // --- Begin I2C Write Transaction ---

    // 1. Generate a START condition on the bus.
    if (I2C1_Start() != I2C_OK) { /* Handle Error: Bus busy or other issue */ return; }

    // 2. Send the 7-bit slave address with the direction bit set to 'write' (0).
    if (I2C1_SendAddress(SENSOR_I2C_ADDR, I2C_TRANSMITTER) != I2C_OK) { /* Handle Error: No ACK from slave */ return; }

    // 3. Send the address of the internal register you want to write to.
    if (I2C1_SendData(SENSOR_CONFIG_REG) != I2C_OK) { /* Handle Error */ return; }

    // 4. Send the data value to be written into that register.
    if (I2C1_SendData(SENSOR_CONFIG_VAL) != I2C_OK) { /* Handle Error */ return; }

    // 5. Generate a STOP condition to release the bus.
    I2C1_Stop();
}
```

#### Example 2: Reading Data (Sensor Identification)
Reading from a register is almost always a two-phase operation: first, a write to tell the slave *which* register you want, followed by a read to get the data.

Here, we read the "WHO_AM_I" register (`0x75`) of a common MPU-6050 sensor (address `0x68`). The sensor should return its own static address, `0x68`, which is a great way to verify communication.

```c
#define MPU6050_I2C_ADDR      0x68
#define MPU6050_WHO_AM_I_REG  0x75

uint8_t Read_Sensor_ID(void) {
    uint8_t received_data = 0;
    // Ensure I2C1 is initialized.

    // --- PHASE 1: Set register pointer (a WRITE operation) ---

    // 1. Generate a START condition.
    if (I2C1_Start() != I2C_OK) return 0;

    // 2. Send the MPU6050 address with the WRITE flag.
    if (I2C1_SendAddress(MPU6050_I2C_ADDR, I2C_TRANSMITTER) != I2C_OK) return 0;

    // 3. Send the address of the WHO_AM_I register.
    if (I2C1_SendData(MPU6050_WHO_AM_I_REG) != I2C_OK) return 0;


    // --- PHASE 2: Read data from the selected register (a READ operation) ---

    // 4. Generate a REPEATED START to switch to read mode without releasing the bus.
    if (I2C1_Start() != I2C_OK) return 0;

    // 5. Send the MPU6050 address again, this time with the READ flag.
    if (I2C1_SendAddress(MPU6050_I2C_ADDR, I2C_RECEIVER) != I2C_OK) return 0;

    // 6. Receive a single byte. Since this is the only (and last) byte, we must
    //    send a NACK (Not Acknowledge) to signal the end of the transfer.
    //    Our library conveniently combines sending NACK and STOP in this call.
    if (I2C1_ReceiveData(I2C_NACK, &received_data) != I2C_OK) return 0;

    // The I2C1_ReceiveData function with the I2C_NACK flag will automatically
    // generate the STOP condition after the byte is read. The bus is now free.

    return received_data; // If successful, this will be 0x68.
}
```
**Note on reading multiple bytes:** If you need to read several consecutive bytes, you would call `I2C1_ReceiveData(I2C_ACK, &data_byte_N)` for all bytes except the last one. For the very last byte, you must use `I2C1_ReceiveData(I2C_NACK, &last_data_byte)` to correctly terminate the transaction.

### SPI Master Communication

The SPI library configures the peripheral in master mode. The Slave Select (NSS) pin is managed manually as a standard GPIO output, giving you full control over which slave is active.

**Example: Sending a command byte (0x9F) to an SPI flash memory and reading its 3-byte JEDEC ID.**

```c
#define CMD_JEDEC_ID 0x9F

void Read_Flash_ID(void) {
    // Ensure SPI1 is initialized: SPI1_Init();
    uint8_t jedec_id[3];
    
    // --- Begin SPI Transaction ---

    // 1. Select the slave device by pulling its Chip Select pin LOW.
    // Our library uses PA4 for this purpose.
    SPI1_EnableSlave();

    // 2. Send the command byte. The data received during this transfer is usually
    //    not useful (often called the MISO-dummy byte) and can be ignored.
    SPI1_Write(CMD_JEDEC_ID);

    // 3. Read the next 3 bytes. To do this, we must send dummy bytes (e.g., 0x00)
    //    to generate the SCK clock signals that allow the slave to send its data.
    jedec_id[0] = SPI1_Write(0x00); // Read Manufacturer ID
    jedec_id[1] = SPI1_Write(0x00); // Read Memory Type
    jedec_id[2] = SPI1_Write(0x00); // Read Capacity

    // 4. Deselect the slave device by pulling its Chip Select pin HIGH.
    // This completes the transaction and releases the slave for other masters.
    SPI1_DisableSlave();

    // 'jedec_id' now contains the 3-byte ID from the flash chip.
    // You can now process this data, e.g., send it over USART.
}
```
