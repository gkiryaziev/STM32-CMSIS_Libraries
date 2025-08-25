
# Interrupt-Driven 2-Digit 7-Segment Display Driver

This library provides a high-level, easy-to-use driver for a 2-digit, multiplexed 7-segment display. It is designed to work with the specific hardware configuration shown below, which uses a `74HC595` shift register to control the segments.

The key feature of this driver is that it uses a **hardware timer interrupt (TIM2)** to handle the display multiplexing automatically in the background. This frees up the main CPU loop from the task of constantly refreshing the display, making your main application code cleaner and more efficient.

## Hardware Setup

This driver is specifically written for the following schematic. It relies on:
1.  A `74HC595` shift register controlling the 8 segments (A-G and DP).
2.  Two GPIO pins (`DIG1`, `DIG2`) controlling high-side switches to select the active digit.



## How It Works: Interrupt-Driven Multiplexing

Instead of manually switching digits in the main `while(1)` loop, this driver automates the process:

1.  `LED_Display_Init()` configures Timer 2 (`TIM2`) to generate an interrupt every 5 milliseconds.
2.  `LED_Display_Write(number)` simply stores the two digits of the number into a buffer. It does not perform any I/O.
3.  The `TIM2_IRQHandler()` function executes automatically every 5ms in the background. Inside the handler:
    *   It turns off both digits to prevent "ghosting".
    *   It selects the next digit to display (0 or 1).
    *   It looks up the correct segment pattern for that digit from the `LED_Display_Digits` array.
    *   It sends this pattern to the `74HC595` shift register.
    *   It turns on the GPIO pin for the currently selected digit.
4.  Because this happens every 5ms (a frequency of 200Hz), the human eye perceives both digits as being solidly lit simultaneously.

## Prerequisites

Your project must have the following libraries and configurations:
*   `RCC_Init()` and `DWT_Init()`.
*   The **low-level `74HC595` driver**, which must be initialized using the **Hardware SPI** method, as the interrupt handler calls `HC595_SPI_SendByte()`.
*   The `TIM2_IRQHandler` must be correctly defined in your startup file (`startup_stm32f103xb.s`) and not used by another part of your application.

## Usage Guide

Using this driver is extremely simple. Once initialized, you can "fire and forget"—the display will continue to show the last number written to it without any further action required in the main loop.

### Complete Example

This example demonstrates how to initialize the system and display a counter from 0 to 99, with each number showing for half a second.

```c
#include "main.h"
#include "74HC595.h"       // Required by the LED Display driver
#include "led_display.h"   // The high-level driver header

int main(void) {
    // 1. System Initialization
    RCC_Init();
    DWT_Init();

    // 2. Low-Level Shift Register Initialization (MUST use SPI)
    SPI1_Init();
    HC595_SPI_Init();

    // 3. High-Level Display Initialization
    // This starts the TIM2 interrupt, and the display will begin multiplexing immediately.
    LED_Display_Init();

    // --- Main Application ---
    // The main loop is now free for other tasks.
    // The display refresh is handled entirely by the TIM2 interrupt.

    for (uint8_t i = 0; i < 100; ++i) {
        
        // Write the number to the display buffer.
        // The interrupt handler will pick it up on its next cycle.
        LED_Display_Write(i);

        // This is a simple blocking delay. Notice there are no calls to an
        // "Update" function here. The display refreshes itself.
        DWT_delay_ms(500);
    }

    // After the loop, the display will continue to show "99" indefinitely.
    while (1) {
        // You can do other things here.
        // For example, check for user input via USART to change the displayed number.
    }
}
```

![Alt text](schematic.png?raw=true "schematic")