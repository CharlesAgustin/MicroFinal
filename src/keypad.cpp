#include <Arduino.h>
#include "keypad.h"
#include "timer.h"

// === Global Variables ===
int currentMode = MODE_NUMERIC;
int selectedNumKey = 0;
int selectedCharIndex = 0;
String currentInput = "";
extern String displayText = "";
// String lastDisplayText = "";

const char* numToAlpha[10][4] = {
  {"a", "b", "c", NULL}, {"d", "e", "f", NULL}, {"g", "h", "i", NULL},
  {"j", "k", "l", NULL}, {"m", "n", "o", NULL}, {"p", "q", "r", "s"},
  {"t", "u", "v", NULL}, {"w", "x", "y", "z"}, {"space", NULL, NULL, NULL},
  {"0", NULL, NULL, NULL}
};

const char* specialChars[] = {
  "!", "@", "#", "$", "%", "^", "&", "*", "(", ")", "-", "_", "+", "=",
  "[", "]", "{", "}", ";", ":", "'", "\"", ",", ".", "/", "?"
};

// bool keyPressed[16] = {false};

void initKeypad() {
  // Set pin 7 (PH4) as OUTPUT (SCL)
  DDRH |= (1 << PH4);

  // Set pin 6 (PH3) as INPUT (SDO)
  DDRH &= ~(1 << PH3);

  // Set SCL (pin 7 / PH4) HIGH
  PORTH |= (1 << PH4);

  delayMs(600);
}


// === Read Keypad ===
int readTTP229TouchKey() {
  uint16_t data = 0;
  for (int i = 0; i < 16; i++) {
    PORTH &= ~(1 << PH4);
    // digitalWrite(TTP229_SCL_PIN, LOW);
    delayUs(3);
    PORTH |= (1 << PH4);
    // digitalWrite(TTP229_SCL_PIN, HIGH);
    delayUs(3);
    data |= (!(PINH & (1 << PH3)) << i);
    // data |= (!digitalRead(TTP229_SDO_PIN) << i);
  }
  for (int i = 0; i < 16; i++) {
    if ((data >> i) & 0x01) return i + 1;
  }
  return 0;
}

// === Key Press Dispatcher ===
void processKeyPress(int key) {
  switch (currentMode) {
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
void processNumericModeKey(int key) {
  if (key >= KEY_NUM_START && key <= 10) {
    int num = (key == 10) ? 0 : key;
    currentInput += String(num);
    displayText = currentInput;
  } else if (key == KEY_ACCEPT) {
    // Serial.println("Input accepted: " + currentInput);
  } else if (key == KEY_DELETE) {
    if (currentInput.length() > 0) {
      currentInput.remove(currentInput.length() - 1);
      displayText = currentInput;
    }
  } else if (key == KEY_ALPHA_TOGGLE) {
    currentMode = MODE_ALPHABET;
    displayText = currentInput + " [ABC]";
  } else if (key == KEY_SPECIAL_TOGGLE) {
    currentMode = MODE_SPECIAL;
    displayText = currentInput + " [#&@]";
  }
}

// === Alphabet Mode ===
void processAlphabetModeKey(int key) {
  if (key >= KEY_NUM_START && key <= 10) {
    selectedNumKey = (key == 10) ? 10 : key;
    selectedCharIndex = 0;
    updateAlphabetDisplay();
  } else if (key == KEY_LEFT || key == KEY_RIGHT) {
    if (selectedNumKey > 0) {
      if (key == KEY_LEFT && selectedCharIndex > 0) selectedCharIndex--;
      if (key == KEY_RIGHT && selectedCharIndex < 3 &&
          numToAlpha[selectedNumKey - 1][selectedCharIndex + 1] != NULL)
        selectedCharIndex++;
      updateAlphabetDisplay();
    }
  } else if (key == KEY_ACCEPT) {
    if (selectedNumKey > 0 && numToAlpha[selectedNumKey - 1][selectedCharIndex]) {
      currentInput += String(numToAlpha[selectedNumKey - 1][selectedCharIndex]);
      selectedNumKey = 0;
      displayText = currentInput;
    }
  } else if (key == KEY_DELETE) {
    if (currentInput.length() > 0) {
      currentInput.remove(currentInput.length() - 1);
      displayText = currentInput;
    }
  } else if (key == KEY_ALPHA_TOGGLE) {
    currentMode = MODE_NUMERIC;
    selectedNumKey = 0;
    displayText = currentInput;
  }
}

// === Update Alphabet Display ===
void updateAlphabetDisplay() {
  String options = "";
  for (int i = 0; i < 4; i++) {
    if (numToAlpha[selectedNumKey - 1][i]) {
      options += String(numToAlpha[selectedNumKey - 1][i]) + " ";
    }
  }
  displayText = currentInput + " [" + options + "] " + numToAlpha[selectedNumKey - 1][selectedCharIndex];
}

// === Special Char Mode ===
void processSpecialModeKey(int key) {
  if (key >= KEY_NUM_START && key <= 10) {
    int index = (key == 10) ? 9 : key - 1;
    if ((unsigned)index < sizeof(specialChars) / sizeof(specialChars[0])) {
      currentInput += String(specialChars[index]);
      displayText = currentInput;
    }
  } else if (key == KEY_ACCEPT) {
    // Serial.println("Input accepted: " + currentInput);
  } else if (key == KEY_DELETE) {
    if (currentInput.length() > 0) {
      currentInput.remove(currentInput.length() - 1);
      displayText = currentInput;
    }
  } else if (key == KEY_SPECIAL_TOGGLE) {
    currentMode = MODE_NUMERIC;
    displayText = currentInput;
  }
}

void clearInput(){
  currentInput = "";
  displayText = "";
  selectedNumKey = 0;
  selectedCharIndex = 0;
  currentMode = MODE_NUMERIC;
}
