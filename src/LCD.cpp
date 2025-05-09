// #include "general.h"
#include "LCD.h"
#include "I2C.h"
#include "timer.h"

#define LCD_ENABLE 0x04
#define LCD_COMMAND 0
#define LCD_DATA 1
#define LCD_BACKLIGHT 0x08

static void pulseEnable(uint8_t data) {
    SendI2C(data | LCD_ENABLE);
    delayUs(1);
    SendI2C(data & ~LCD_ENABLE);
    delayUs(50);
}

static void send4Bits(uint8_t data) {
    SendI2C(data | LCD_BACKLIGHT);
    pulseEnable(data);
}

static void sendByte(uint8_t value, uint8_t mode) {
    uint8_t highNib = value & 0xF0;
    uint8_t lowNib = (value << 4) & 0xF0;

    send4Bits(highNib | (mode << 0));
    send4Bits(lowNib | (mode << 0));
}

void lcdSetCursor(uint8_t row, uint8_t col) {
    uint8_t address;

    // Ensure row is either 0 or 1
    if (row == 0) {
        address = col; // Row 0 starts at 0x00
    } else if (row == 1) {
        address = 0x40 + col; // Row 1 starts at 0x40
    } else {
        return; // Invalid row, do nothing
    }

    // Set the cursor position by sending the DDRAM address
    sendByte(0x80 | address, LCD_COMMAND);
}

void lcdClear() {
    sendByte(0x01, LCD_COMMAND);  // Send the clear display command
    delayMs(2);  // Wait for the display to clear
}



void lcdInit() {
    InitI2CPin();
    delayMs(50);
    send4Bits(0x30); delayMs(5);
    send4Bits(0x30); delayMs(5);
    send4Bits(0x30); delayMs(1);
    send4Bits(0x20);

    sendByte(0x28, LCD_COMMAND);
    sendByte(0x0C, LCD_COMMAND);
    sendByte(0x01, LCD_COMMAND);
    delayMs(2);
    sendByte(0x06, LCD_COMMAND);
}

void lcdPrint(const char* str) {
    while (*str) {
        sendByte(*str++, LCD_DATA);
    }
}

void lcdBacklight(bool on) {
    extern uint8_t _backlightVal;
    _backlightVal = on ? LCD_BACKLIGHT : 0x00;
}