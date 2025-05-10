#include <Arduino.h>
#ifndef KEYPAD_H
#define KEYPAD_H

#define TTP229_SCL_PIN 7 // SCL -> SCL pin on Arduino Mega
#define TTP229_SDO_PIN 6 // SDO -> SDA pin on Arduino Mega

// Constants for key mappings
#define KEY_NUM_START 1
#define KEY_ACCEPT 11
#define KEY_DELETE 12
#define KEY_ALPHA_TOGGLE 13
#define KEY_SPECIAL_TOGGLE 14
#define KEY_LEFT 15
#define KEY_RIGHT 16

#define MODE_NUMERIC 0
#define MODE_ALPHABET 1
#define MODE_SPECIAL 2

extern int currentMode;
extern String currentInput;
extern String displayText;
extern String lastDisplayText;
// extern bool keyPressed[16];

void initKeypad();
int readTTP229TouchKey();
void processKeyPress(int key);
void processNumericModeKey(int key);
void processAlphabetModeKey(int key);
void processSpecialModeKey(int key);
void updateAlphabetDisplay();
void updateSpecialDisplay();
void clearInput();

#endif
