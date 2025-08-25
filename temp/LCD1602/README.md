
# HD44780 LCD 16x2 / 20x4 Character Display Drivers

This directory contains drivers for the common HD44780-compatible character LCDs (e.g., 16x2 or 20x4 displays). The library supports three common methods of connection:

1.  **I2C Mode**: Uses a PCF8574T I/O expander to control the LCD with only two I2C wires (SCL/SDA). This is the most pin-efficient method.
2.  **4-Bit Parallel Mode**: Controls the LCD using 6 GPIO pins. This is a balance between pin usage and speed.
3.  **8-Bit Parallel Mode**: Controls the LCD using 10 GPIO pins. This is the fastest method but uses the most pins.

The library also includes helpers for creating and displaying up to 8 custom characters.

## Common Prerequisites

Regardless of the connection method, your project must be initialized with the base libraries:
*   `RCC_Init()` to set up the system clock.
*   `DWT_Init()` as all drivers use DWT-based delays.

---

## 1. I2C Mode (via PCF8574T I/O Expander)

This is the recommended method for most projects as it saves a significant number of GPIO pins. It relies on the `PCF8574T` driver.

### Hardware Setup
You will need an I2C LCD adapter module, which is typically soldered to the back of the LCD. Connect it to your STM32 as follows:
*   `VCC` -> 5V
*   `GND` -> GND
*   `SDA` -> `PB7` (I2C1 SDA)
*   `SCL` -> `PB6` (I2C1 SCL)

![Alt text](lcd1602_pcf8574t.png?raw=true "lcd1602_pcf8574t")

### Usage Example

```c
#include "main.h"
#include "lcd1602_i2c.h" // Use the I2C-specific header

int main(void) {
    // 1. Initialize system clock and delays
    RCC_Init();
    DWT_Init();
    
    // 2. Initialize the I2C peripheral
    I2C1_Init(I2C_STANDARD);
    
    // 3. Initialize the LCD via I2C, turning the backlight ON
    LCD1602_Init(LCD1602_LED_ON);

    // 4. Start using the LCD
    LCD1602_Clear();
    LCD1602_CursorPos(0, 0); // Row 0, Column 0
    LCD1602_SendString("Hello, World!");

    LCD1602_CursorPos(1, 4); // Row 1, Column 4
    LCD1602_SendString("I2C Mode");
    
    // Control the backlight
    DWT_delay_ms(2000);
    LCD1602_Led(LCD1602_LED_OFF);
    DWT_delay_ms(1000);
    LCD1602_Led(LCD1602_LED_ON);

    while(1) {
        // Your application code
    }
}
```

---

## 2. 4-Bit Parallel Mode

This mode provides a good balance between performance and pin usage.

### Hardware Setup
Connect the LCD directly to your STM32's GPIO pins. The default configuration in `lcd1602_4bit.h` uses 6 pins:
*   `RS` -> `PB4`
*   `E`  -> `PB3`
*   `D4` -> `PB12`
*   `D5` -> `PB13`
*   `D6` -> `PB14`
*   `D7` -> `PB15`
*   `RW` should be connected to `GND`.

### Usage Example

```c
#include "main.h"
#include "lcd1602_4bit.h" // Use the 4-bit specific header

int main(void) {
    // 1. Initialize system clock and delays
    RCC_Init();
    DWT_Init();
    
    // 2. Initialize the LCD in 4-bit mode.
    // The library handles GPIO configuration internally.
    LCD1602_Init();

    // 3. Start using the LCD
    LCD1602_Clear();
    LCD1602_CursorPos(0, 0);
    LCD1602_SendString("Hello, World!");

    LCD1602_CursorPos(1, 3);
    LCD1602_SendString("4-Bit Mode");

    while(1) {
        // Your application code
    }
}
```

---

## 3. 8-Bit Parallel Mode

This mode offers the highest performance at the cost of using many GPIO pins.

### Hardware Setup
Connect the LCD directly to your STM32's GPIO pins. The default configuration in `lcd1602_8bit.h` uses 10 pins:
*   `RS` -> `PB4`
*   `E`  -> `PB3`
*   `D0` -> `PB8`
*   `D1` -> `PB9`
*   `D2` -> `PB10`
*   `D3` -> `PB11`
*   `D4` -> `PB12`
*   `D5` -> `PB13`
*   `D6` -> `PB14`
*   `D7` -> `PB15`
*   `RW` should be connected to `GND`.

### Usage Example

```c
#include "main.h"
#include "lcd1602_8bit.h" // Use the 8-bit specific header

int main(void) {
    // 1. Initialize system clock and delays
    RCC_Init();
    DWT_Init();
    
    // 2. Initialize the LCD in 8-bit mode.
    LCD1602_Init();

    // 3. Start using the LCD
    LCD1602_Clear();
    LCD1602_CursorPos(0, 0);
    LCD1602_SendString("Hello, World!");

    LCD1602_CursorPos(1, 3);
    LCD1602_SendString("8-Bit Mode");

    while(1) {
        // Your application code
    }
}
```

---

## 4. Creating and Using Custom Characters

All driver versions support creating up to 8 custom characters (locations 0-7) and storing them in the LCD's CGRAM. This example uses the `lcd1602_char.h` helper file, which pre-defines some useful character patterns.

This feature works with any of the connection modes (I2C, 4-bit, or 8-bit).

### Usage Example

```c
#include "main.h"
#include "lcd1602_i2c.h"    // Or any other LCD driver header
#include "lcd1602_char.h"   // Include the custom character definitions

int main(void) {
    // Standard initialization for your chosen mode (I2C in this case)
    RCC_Init();
    DWT_Init();
    I2C1_Init(I2C_STANDARD);
    LCD1602_Init(LCD1602_LED_ON);
    
    // --- Create and display a custom character ---

    // 1. Create the character and save it to a location in the LCD's memory.
    // We use the pre-defined BATTARY pattern and save it to location 4 (which matches the enum value).
    // The lcd1602CustomChars array holds pointers to the patterns.
    LCD1602_CreateChar(lcd1602CustomChars[LCD1602_BATTARY], LCD1602_BATTARY);
    
    // Create another character, a smiley face, and save it to location 0.
    LCD1602_CreateChar(lcd1602CustomChars[LCD1602_SMILE], LCD1602_SMILE);

    // 2. Display the characters on the screen
    LCD1602_Clear();
    LCD1602_CursorPos(0, 0);
    LCD1602_SendString("Custom Chars: ");
    
    // To display a custom character, send its memory location (0-7) as data.
    LCD1602_SendChar(LCD1602_BATTARY); // Display the battery
    LCD1602_SendChar(LCD1602_SMILE);   // Display the smiley face

    while(1) {
        // Your application code
    }
}
```
