#ifndef LCD_H
#define LCD_H
#include <avr/io.h>
#include "general.h"
#include "I2C.h"

void initPinLCD();
void writeCharacter(unsigned char character);
void writeString(const char *string);
void moveCursor(unsigned char x, unsigned char y);
void initLCD(); // call initPinLCD and initProcedure
void initLCDProcedure();

#endif
