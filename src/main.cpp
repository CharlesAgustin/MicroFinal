// Authors: I MADE KENT ABIATAR WIRANATHA, ANDREW LIEMARTA, BARI FAKHRUDIN, CHARLES AGUSTIN
// Net ID: kentabiatar,andrewliemarta,barifakhrudin,charlesagustin
// Date: 22-Apr-2025
// Assignment: Final Project (Snack Safe)
//----------------------------------------------------------------------//
#include <Arduino.h>
#include <avr/interrupt.h>
#include "I2C.h"
#include "keypad.h"
#include "LCD.h"
#include "motor.h"
#include "PWM.h"
#include "timer.h"

// full testing main
extern String displayText;
String lastDisplayText = "";
bool keyPressed[16] = {false};
bool enteringPasswordMode = false;

int main()
{
  initTimer1();
  initTimer0();
  initPWMTimer3();
  initMotorPins();
  sei();
  lcdInit();
  delayMs(1000);
  lcdBacklight(true);
  initKeypad();
  initBuzzerPin();
  initPWMTimer2();
  Serial.begin(9600);                                // debug
  Serial.print("Finished initializing no probs \n"); // debug

  String password = ""; // initally the password is empty
  bool locked = false;  // initally the safe is opened
  int wrongTries = 3;   // given 3 tries to input the right password.

  while (1)
  {

    // IF IT IS FIRST ATTEMPT
    if (password.length() < 5) // make sure password is 4 character
    {
      lcdSetCursor(0, 0);
      lcdPrint("Set Password");

      int key = readTTP229TouchKey();
      if (key > 0 && !keyPressed[key - 1])
      {
        if (displayText.length() < 5 || key == KEY_DELETE || key == KEY_ACCEPT)
        {
          processKeyPress(key);
        }
        else
        {
          lcdSetCursor(0, 1);
          lcdPrint("Max 4 chars only"); // display hints
          delayMs(1000);                // Show warning briefly
        }
        keyPressed[key - 1] = true;
        delayMs(300);
      }
      else if (key == 0)
      {
        for (int i = 0; i < 16; i++)
          keyPressed[i] = false;
      }

      if (key == KEY_ACCEPT)
      {
        password = displayText;
        setMotorDirection(90);
        displayText = "";
        delayMs(100);
        lastDisplayText = displayText;
        lcdClear();
        clearInput();
        locked = true;
      }

      if (displayText != lastDisplayText)
      {
        Serial.print("Input: ");
        Serial.println(displayText);
        lcdClear();
        lcdSetCursor(0, 0);
        lcdPrint("Set Password");
        lcdSetCursor(1, 0);
        lcdPrint(displayText.c_str());
        lastDisplayText = displayText;
      }
    }
    // IF IT IS IN LOCKED MODE
    else if (locked == true && wrongTries != 0)
    {

      lcdSetCursor(0, 0);
      lcdPrint("Enter Password");

      int key = readTTP229TouchKey();
      if (key > 0 && !keyPressed[key - 1])
      {
        if (displayText.length() < 5 || key == KEY_DELETE || key == KEY_ACCEPT)
        {
          processKeyPress(key);
        }
        else
        {
          lcdSetCursor(0, 1);
          lcdPrint("Max 4 chars only"); // display hints
          delayMs(1000);                // Show warning briefly
        }
        keyPressed[key - 1] = true;
        delayMs(300);
      }
      else if (key == 0)
      {
        for (int i = 0; i < 16; i++)
          keyPressed[i] = false;
      }

      if (key == KEY_ACCEPT)
      {
        if (displayText != password)
        {
          lcdSetCursor(0, 0);
          lcdPrint("Wrong Password");
          delayMs(500);
          wrongTries--;
          lcdSetCursor(1, 0);
          String triesMsg = "Tries Left " + String(wrongTries);
          lcdPrint(triesMsg.c_str());
          delayMs(500);
        }
        else
        {
          lcdSetCursor(0, 0);
          lcdPrint("Correct");
          setMotorDirection(-90);
          locked = false;
          delayMs(500);
        }

        clearInput();
        lcdClear();
      }

      if (displayText != lastDisplayText)
      {
        Serial.print("Input: ");
        Serial.println(displayText);
        lcdClear();
        lcdSetCursor(0, 0);
        lcdPrint("Enter Password");
        lcdSetCursor(1, 0);
        lcdPrint(displayText.c_str());
        lastDisplayText = displayText;
      }

      // IF IT IS IN UNLOCK MODE
    }
    else if (locked == false && wrongTries != 0)
    {
      lcdSetCursor(0, 0);
      lcdPrint("Press Enter");
      lcdSetCursor(1, 0);
      lcdPrint("To Lock");

      int key = readTTP229TouchKey();
      if (key > 0 && !keyPressed[key - 1])
      {
        if (displayText.length() < 5 || key == KEY_DELETE || key == KEY_ACCEPT)
        {
          processKeyPress(key);
        }
        else
        {
          lcdSetCursor(0, 1);
          lcdPrint("Max 4 chars only"); // display hints
          delayMs(1000);                // Show warning briefly
        }
        keyPressed[key - 1] = true;
        delayMs(300);
      }
      else if (key == 0)
      {
        for (int i = 0; i < 16; i++)
          keyPressed[i] = false;
      }

      if (key == KEY_ACCEPT)
      {
        lcdClear();
        lcdSetCursor(0, 0);
        lcdPrint("Locked");
        setMotorDirection(90);
        locked = true;
        delayMs(500);
        clearInput();
      }

      // IF IN SHUTDOWN MODE
    }
    else
    {

      lcdSetCursor(0, 0);
      lcdPrint("You're not the");
      lcdSetCursor(1, 0);
      lcdPrint("OWNER!!");
      delayMs(1000);

      for (int i = 0; i < 10; i++)
      { // stuck for 3 minutes, for now just 10 sec
        if (i <= 5)
        { // turn buzzer for 30 second for now 5 sec
          chirpBuzzer(0.9);
        }

        lcdSetCursor(0, 0);
        lcdPrint("Please wait for");
        String waitMsg = String(10 - i) + " Sec";
        lcdSetCursor(1, 0);
        lcdPrint(waitMsg.c_str());
        delayMs(1000); // delay 1 second
        lcdClear();
      }

      wrongTries = 3;
      turnOffBuzzer();
      lcdClear();
    }
  }
  return 0;
}
