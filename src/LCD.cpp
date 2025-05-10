// #include "general.h"
#include "LCD.h"
#include "I2C.h"
#include "timer.h"

// Define constants used in LCD commands
#define LCD_ENABLE 0x04    // Enable bit used to latch data into the LCD
#define LCD_COMMAND 0      // Flag indicating the byte is a command
#define LCD_DATA 1         // Flag indicating the byte is data
#define LCD_BACKLIGHT 0x08 // Bit to turn on the LCD backlight

static void pulseEnable(uint8_t data) // send pulse to enable  latch
{
    SendI2C(data | LCD_ENABLE);  // set enable bit to high
    delayUs(1);                  // give 1 microsecond delay
    SendI2C(data & ~LCD_ENABLE); // set enable bit to low  to stop transmission
    delayUs(50);                 // give 50 microsecond delay
}

static void send4Bits(uint8_t data) // Sends 4 bits of data to the LCD
{
    SendI2C(data | LCD_BACKLIGHT); // Write data with backlight status
    pulseEnable(data);             // Pulse the ENABLE line to latch the data
}

static void sendByte(uint8_t value, uint8_t mode) // Sends a full 8-bit byte to the LCD by splitting it into two 4-bit transmissions
{
    uint8_t first4bit = value & 0xF0;       // Extract the upper 4 bits
    uint8_t last4bit = (value << 4) & 0xF0; // Extract the lower 4 bits

    send4Bits(first4bit | (mode << 0)); // Send the first 4 bits with mode (data/command)
    send4Bits(last4bit | (mode << 0));  // Send the last 4 bits with mode (data/command)
}

void lcdSetCursor(uint8_t row, uint8_t col) // Sets the cursor position on the LCD
{
    uint8_t address;

    // Ensure row is either 0 or 1
    if (row == 0)
    {
        address = col; // Row 0 starts at 0x00
    }
    else if (row == 1)
    {
        address = 0x40 + col; // Row 1 starts at 0x40
    }
    else
    {
        return; // Invalid row, do nothing
    }

    // Set the cursor position by sending the DDRAM address
    sendByte(0x80 | address, LCD_COMMAND);
}

void lcdClear() // Clears the LCD screen
{
    sendByte(0x01, LCD_COMMAND); // Send the clear display command
    delayMs(2);                  // Wait for the display to clear
}

void lcdInit() // Initializes the LCD
{
    InitI2CPin();    // Initialize I2C communication
    delayMs(50);     // give 50 microsecond delay
    send4Bits(0x30); // Initialization sequence (send 0x30 three times to make sure LCD in 8 bit mode)
    delayMs(5);      // give 5 milisecond delay
    send4Bits(0x30);
    delayMs(5);
    send4Bits(0x30);
    delayMs(1);      // give 1 milisecond delay
    send4Bits(0x20); // Set to 4-bit mode

    sendByte(0x28, LCD_COMMAND); // Function set: 4-bit, 2 lines, 5x8 dots
    sendByte(0x0F, LCD_COMMAND); // Display ON, cursor ON, blink ON
    sendByte(0x01, LCD_COMMAND); // Clear display
    delayMs(2);                  // give 2 milisecond delay
    sendByte(0x06, LCD_COMMAND); // Entry mode set: move cursor right, no shift
}

void lcdPrint(const char *str) // Prints a null-terminated string to the LCD
{
    while (*str)
    {
        sendByte(*str++, LCD_DATA); // Send each character as data
    }
}

void lcdBacklight(bool on) // Turns LCD backlight on or off
{
    extern uint8_t _backlightVal;              // Use external variable to store backlight status
    _backlightVal = on ? LCD_BACKLIGHT : 0x00; // Set backlight bit based on input
}