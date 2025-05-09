#ifndef LCD_H
#define LCD_H
#include <avr/io.h>
#include "I2C.h"
// https://wokwi.com/projects/429122725257792513
// void initPinLCD();
// void writeCharacter(unsigned char character);
// void writeString(const char *string);
// void moveCursor(unsigned char x, unsigned char y);
// void initLCD(); // call initPinLCD and initProcedure
// void initLCDProcedure();

static void pulseEnable(uint8_t data);
static void send4Bits(uint8_t data);
static void sendByte(uint8_t value, uint8_t mode);
void lcdSetCursor(uint8_t row, uint8_t col);
void lcdClear(); 
void lcdInit();
void lcdPrint(const char* str);
void lcdBacklight(bool on);


#endif
