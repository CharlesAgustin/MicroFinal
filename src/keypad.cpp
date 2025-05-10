#include <Arduino.h>
#include "keypad.h"
#include "timer.h"

// === Global Variables ===
int currentMode = MODE_NUMERIC;   // set the mode to be numeric by default
int selectedNumKey = 0;           // 0 key is pressed
int selectedCharIndex = 0;        // index 0 for character
String currentInput = "";         // empty password
extern String displayText = "";   // no password is shown
int selectedSpecialCharIndex = 0; // index into specialChars[]

const char *numToAlpha[10][4] = { // alphabet mapping (e.g. abc on key 1)
    {"a", "b", "c", NULL},        // key number 1
    {"d", "e", "f", NULL},        // key number 2
    {"g", "h", "i", NULL},        // key number 3
    {"j", "k", "l", NULL},        // key number 4
    {"m", "n", "o", NULL},        // key number 5
    {"p", "q", "r", "s"},         // key number 6
    {"t", "u", "v", NULL},        // key number 7
    {"w", "x", "y", "z"},         // key number 8
    {"space", NULL, NULL, NULL},  // key number 9
    {"0", NULL, NULL, NULL}};     // key number 10

const char *specialChars[10][3] = { // special character mapping (e.g. ! on key 1)
    {"!", "@", "#"},                // key number 1
    {"$", "%", "^"},                // key number 2
    {"&", "*", "'"},                // key number 3
    {"-", "_", "+"},                // key number 4
    {"=", "[", "]"},                // key number 5
    {"{", "}", ";"},                // key number 6
    {"(", ")", ":"},                // key number 7
    {",", ".", "\""},               // key number 8
    {"/", "?", NULL},               // key number 9
    {"0", NULL, NULL}};             // key number 10

void initKeypad()
{ // initialize the pins for the keypad
  // Set pin 7 (PH4) as OUTPUT (SCL)
  DDRH |= (1 << PH4);

  // Set pin 6 (PH3) as INPUT (SDO)
  DDRH &= ~(1 << PH3);

  // Set SCL (pin 7 / PH4) HIGH
  PORTH |= (1 << PH4);

  delayMs(600);
}

// === Read Keypad ===
int readTTP229TouchKey() // read the keypad input
{
  uint16_t data = 0;
  for (int i = 0; i < 16; i++)
  {
    PORTH &= ~(1 << PH4);
    // digitalWrite(TTP229_SCL_PIN, LOW);
    delayUs(3);
    PORTH |= (1 << PH4);
    // digitalWrite(TTP229_SCL_PIN, HIGH);
    delayUs(3);
    data |= (!(PINH & (1 << PH3)) << i);
    // data |= (!digitalRead(TTP229_SDO_PIN) << i);
  }
  for (int i = 0; i < 16; i++) // Return the value of the key that is currently pressed
  {
    if ((data >> i) & 0x01)
      return i + 1;
  }
  return 0;
}

// === Key Press Dispatcher ===
void processKeyPress(int key) // checking which mode the keypad in
{
  switch (currentMode)
  {
  case MODE_NUMERIC:
    processNumericModeKey(key);
    break;
  case MODE_ALPHABET:
    processAlphabetModeKey(key);
    break;
  case MODE_SPECIAL:
    processSpecialModeKey(key);
    break;
  }
}

// === Numeric Mode ===
void processNumericModeKey(int key)
{
  if (key >= KEY_NUM_START && key <= 10) // ensure that the key pressed is between 1 until 10 (key inputs are only from key number 1 until 10)
  {
    int num = (key == 10) ? 0 : key; // set the key number 10 to be 0
    currentInput += String(num);     // add the input to password
    displayText = currentInput;      // display the updated password
  }

  else if (key == KEY_ACCEPT) // if the user press key number 11 (accept key)
  {
    Serial.println("Input accepted: " + currentInput); // for debug checking the input
  }

  else if (key == KEY_DELETE) // if the user press the key number 12 (backspace key)
  {
    if (currentInput.length() > 0) // check if password is not empty
    {
      currentInput.remove(currentInput.length() - 1); // remove the character from the password
      displayText = currentInput;                     // display the updated password
    }
  }

  else if (key == KEY_ALPHA_TOGGLE) // if user press the key number 13 (alphabet toggle)
  {
    currentMode = MODE_ALPHABET;           // change the mode to alphabet mode
    displayText = currentInput + " [ABC]"; // display the current password with [ABC] indicator to indicate in alphabet mode
  }

  else if (key == KEY_SPECIAL_TOGGLE) // if user press the key number 14 (special character toggle)
  {
    currentMode = MODE_SPECIAL;            // change the mode to special character mode
    displayText = currentInput + " [#&@]"; // display the current password with [#&@] indicator to indicate in special character mode
  }
}

// === Alphabet Mode ===
void processAlphabetModeKey(int key)
{
  if (key >= KEY_NUM_START && key <= 10) // ensure that the key pressed is between 1 until 10 (key inputs are only from key number 1 until 10)
  {
    selectedNumKey = (key == 10) ? 0 : key; // set the key number 10 in alphabet mode to be 0 (null) indicating no character is selected
    selectedCharIndex = 0;                  // for choosing the alphabet
    updateAlphabetDisplay();                // display selected alphabet
  }

  else if (key == KEY_LEFT || key == KEY_RIGHT) // process the arrow key (key 15 left key 16 right)
  {
    if (selectedNumKey > 0) // check whether it is already selecting something (e.g any key from 1-9 has been pressed)
    {
      if (key == KEY_LEFT && selectedCharIndex > 0) // if left key (key 15) is pressed and the index is not leftmost
        selectedCharIndex--;                        // move the index to the left
      if (key == KEY_RIGHT && selectedCharIndex < 3 &&
          numToAlpha[selectedNumKey - 1][selectedCharIndex + 1] != NULL) // if right key (key 16) is pressed and the index is not the right most and the next index is not NULL
        selectedCharIndex++;                                             // move the index to the right
      updateAlphabetDisplay();                                           // display selected alphabet
    }
  }

  else if (key == KEY_ACCEPT) // if accept key (key 11) is pressed
  {
    if (selectedNumKey > 0 && numToAlpha[selectedNumKey - 1][selectedCharIndex]) // check if a number key is currently selected AND that the selected character is not NULL
    {
      currentInput += String(numToAlpha[selectedNumKey - 1][selectedCharIndex]); // add the selected character to the password
      selectedNumKey = 0;                                                        // reset the selected key
      displayText = currentInput;                                                // display the updated password
    }
  }

  else if (key == KEY_DELETE) // if backspace key (key 12) is pressed
  {
    if (currentInput.length() > 0) // check if password is not empty
    {
      currentInput.remove(currentInput.length() - 1); // remove the character on the password
      displayText = currentInput;                     // display the updated password
    }
  }

  else if (key == KEY_ALPHA_TOGGLE) // pressing the alphabet toggle key (key 13)
  {
    currentMode = MODE_NUMERIC; // return to number mode
    selectedNumKey = 0;         // reset selected key
    displayText = currentInput; // display the current password
  }
}

// === Update Alphabet Display ===
void updateAlphabetDisplay()
{
  String options = "";        // initialize an empty string to store all character options for the selected key
  for (int i = 0; i < 4; i++) // loop through up to 4 possible characters mapped to the selected number key
  {
    if (numToAlpha[selectedNumKey - 1][i]) //// check if a character exists at this position
    {
      options += String(numToAlpha[selectedNumKey - 1][i]) + " "; // append the character to the empty string
    }
  }
  displayText = currentInput + " [" + options + "] " + numToAlpha[selectedNumKey - 1][selectedCharIndex]; // display the options
}

// === Special Char Mode ===
void processSpecialModeKey(int key)
{
  if (key >= KEY_NUM_START && key <= 10) // ensure that the key pressed is between 1 until 10 (key inputs are only from key number 1 until 10)
  {
    selectedNumKey = (key == 10) ? 0 : key; // set the key number 10 in special character mode to be 0 (null) indicating no character is selected
    selectedCharIndex = 0;                  // for choosing the special character
    updateSpecialDisplay();                 // display selected special character
  }

  else if (key == KEY_LEFT || key == KEY_RIGHT) // process the arrow key (key 15 left key 16 right)
  {
    if (selectedNumKey > 0) // check whether it is already selecting something (e.g any key from 1-9 has been pressed)
    {
      if (key == KEY_LEFT && selectedCharIndex > 0) // if left key (key 15) is pressed and the index is not leftmost
        selectedCharIndex--;                        // move the index to the left
      if (key == KEY_RIGHT && selectedCharIndex < 2 &&
          specialChars[selectedNumKey - 1][selectedCharIndex + 1] != NULL) // if right key (key 16) is pressed and the index is not the right most and the next index is not NULL
        selectedCharIndex++;                                               // move the index to the right
      updateAlphabetDisplay();                                             // display selected special character
    }
  }

  else if (key == KEY_ACCEPT) // if accept key (key 11) is pressed
  {
    Serial.println("Input accepted: " + currentInput);                             // for debug
    if (selectedNumKey > 0 && specialChars[selectedNumKey - 1][selectedCharIndex]) // check if a number key is currently selected AND that the selected character is not NULL
    {
      currentInput += String(specialChars[selectedNumKey - 1][selectedCharIndex]); // add the selected character to the password
      selectedNumKey = 0;                                                          // reset the selected key
      displayText = currentInput;                                                  // display the updated password
    }
  }

  else if (key == KEY_DELETE) // if backspace key (key 12) is pressed
  {
    if (currentInput.length() > 0) // check if password is not empty
    {
      currentInput.remove(currentInput.length() - 1); // remove the character on the password
      displayText = currentInput;                     // display the updated password
    }
  }

  else if (key == KEY_SPECIAL_TOGGLE) // pressing the special character toggle key (key 14)
  {
    currentMode = MODE_NUMERIC; // return to number mode
    displayText = currentInput; // display the current password
  }
}

// === Update Special Char Display ===
void updateSpecialDisplay()
{
  String options = "";        // initialize an empty string to store all character options for the selected key
  for (int i = 0; i < 3; i++) // loop through up to 3 possible characters mapped to the selected number key
  {
    if (specialChars[selectedNumKey - 1][i]) //// check if a character exists at this position
    {
      options += String(specialChars[selectedNumKey - 1][i]) + " "; // append the character to the empty string
    }
  }
  displayText = currentInput + " [" + options + "] " + specialChars[selectedNumKey - 1][selectedCharIndex]; // display the options
}

void clearInput() // clear input function
{
  currentInput = "";          // reset the password
  displayText = "";           // clear the display
  selectedNumKey = 0;         // reset the key input
  selectedCharIndex = 0;      // reset index
  currentMode = MODE_NUMERIC; // set the mode to numeric (default is numeric)
}
