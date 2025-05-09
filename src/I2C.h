#ifndef I2C_H
#define I2C_H
#include <avr/io.h>

// LCD address
#define LCD_ADD 0x27

// LCD setup
#define LCD_BACKLIGHT 0x08
#define LCD_ENABLE 0x04
#define LCD_COMMAND 0
#define LCD_DATA 1

void InitI2CPin();
void StartI2CTrans(uint8_t address);
void StopI2CTrans();
void Write(uint8_t data);
void SendI2C(uint8_t value);

#endif