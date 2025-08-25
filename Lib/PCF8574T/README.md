
### PCF8574T I/O Expander Driver

This library is a higher-level driver for the PCF8574(T) 8-bit I2C I/O expander. It builds upon the `I2C` library to provide simple functions for controlling the 8 pins of the expander.

A key feature of this driver is a local software cache of the port's state. When you call `PCF8574T_WritePin`, it modifies this cached value first and then writes the entire new 8-bit state to the device. This approach is efficient as it doesn't require reading the port state from the device before every write.

**Prerequisites:** Your project must be initialized with `RCC_Init()`, `DWT_Init()`, and `I2C1_Init()`. The I/O expander must be connected to the I2C1 pins (PB6/PB7).

---
#### Example 1: Blinking an LED on Pin P0

This example shows how to configure an expander pin as an output and blink an LED connected to it.

```c
// This code can be placed inside your main() function.

// First, check if the PCF8574T device is present on the I2C bus.
// This is good practice for ensuring your hardware is connected correctly.
if (PCF8574T_GetStatus() == I2C_OK) {
    // Device acknowledged its address, communication is likely working.
    USART1_SendString("PCF8574T I/O Expander found!\n");
} else {
    USART1_SendString("Error: PCF8574T not found on the I2C bus.\n");
    // Halt or handle the error, as subsequent calls will fail.
    while(1);
}


// This is the main application loop.
while (1) {
    // Set pin P0 of the expander HIGH.
    // The library handles the read-modify-write logic using its internal cache.
    PCF8574T_WritePin(0, 1);
    DWT_delay_ms(500); // Wait for half a second.

    // Set pin P0 of the expander LOW.
    PCF8574T_WritePin(0, 0);
    DWT_delay_ms(500); // Wait for another half a second.
}
```

---
#### Example 2: Reading a Button on Pin P7

The pins of the PCF8574 are "quasi-bidirectional". To use a pin as an input, you must first write a '1' to it, which puts it in a high-impedance state with a weak pull-up.

This example reads the state of a button connected between pin P7 and GND.

```c
// In your initialization code, after finding the device:
// Set pin P7 to '1' to configure it as an input with a weak pull-up.
PCF8574T_WritePin(7, 1);


// Inside your main while(1) loop:
uint8_t button_state = 0;

// Read the current state of pin P7 into the 'button_state' variable.
// This function performs a full I2C read from the device.
if (PCF8574T_ReadPin(7, &button_state) == I2C_OK) {
    // Check if the button is pressed (pin is pulled to GND).
    if (button_state == 0) {
        USART1_SendString("Button on P7 is pressed!\n");
        // Add a small delay to "debounce" and prevent spamming the serial port.
        DWT_delay_ms(100);
    }
}

// Other non-blocking tasks can run here...
```
![Alt text](pcf8574t_status.png?raw=true "pcf8574t_status")
