#include <Arduino.h>
#include <avr/interrupt.h>
#include "buzzer.h"
#include "I2C.h"
#include "keypad.h"
#include "LCD.h"
#include "motor.h"
#include "PWM.h"
#include "timer.h"



// // full testing main
// extern String displayText;
// String lastDisplayText = "";
// bool keyPressed[16] = {false};
// bool enteringPasswordMode = false;


// int main(){
//   initTimer1();
//   initTimer0();
//   initPWMTimer3();
//   initMotorPins();
//   sei();
//   lcdInit();             
//   delayMs(1000);
//   lcdBacklight(true); 
//   initKeypad(); 
//   initBuzzerPin();
//   initPWMTimer2();
//   Serial.begin(9600);
//   Serial.print("Finished initializing no probs \n");
  
//   String password = "";
//   bool locked = false;
//   int wrongTries = 3;
  
//   while(1){
    
//     // IF IT IS FIRST ATTEMPT
//     if (password.length() < 5) { // make sure password is 4 or less digit long
//       lcdSetCursor(0, 0);
//       lcdPrint("Set Password");
      
//       int key = readTTP229TouchKey();
//       if (key > 0 && !keyPressed[key - 1]) {
//         processKeyPress(key);
//         keyPressed[key - 1] = true;
//         delayMs(300);
//       } else if (key == 0) {
//         for (int i = 0; i < 16; i++) keyPressed[i] = false;
//       }

//       if(key == KEY_ACCEPT){
//         password = displayText;
//         setMotorDirection(90);
//         displayText = "";
//         delayMs(100);
//         lastDisplayText = displayText;
//         lcdClear();
//         clearInput();
//         locked = true;
//       }

//       if (displayText != lastDisplayText) {
//         Serial.print("Input: ");
//         Serial.println(displayText);
//         lcdClear();
//         lcdSetCursor(0, 0);
//         lcdPrint("Set Password");
//         lcdSetCursor(1, 0);
//         lcdPrint(displayText.c_str());
//         lastDisplayText = displayText;
//       }


//       // IF IT IS IN LOCKED MODE
//   } else if(locked == true && wrongTries != 0){

//     lcdSetCursor(0, 0);
//     lcdPrint("Enter Password");

//     int key = readTTP229TouchKey();
//     if (key > 0 && !keyPressed[key - 1]) {
//       processKeyPress(key);
//       keyPressed[key - 1] = true;
//       delayMs(300);
//     } else if (key == 0) {
//       for (int i = 0; i < 16; i++) keyPressed[i] = false;
//     }

//     if(key == KEY_ACCEPT){
//       if (displayText != password){
//         lcdSetCursor(0, 0);
//         lcdPrint("Wrong Password");
//         delayMs(500);
//         wrongTries--;
//         lcdSetCursor(1, 0);
//         String triesMsg = "Tries Left " + String(wrongTries);
//         lcdPrint(triesMsg.c_str());
//         delayMs(500);
//       }
//       else{
//         lcdSetCursor(0, 0);
//         lcdPrint("Correct");
//         setMotorDirection(-90);
//         locked = false;
//         delayMs(500);

//       }

//       clearInput();
//       lcdClear();
//     }

//     if (displayText != lastDisplayText) {
//       Serial.print("Input: ");
//       Serial.println(displayText);
//       lcdClear();
//       lcdSetCursor(0, 0);
//       lcdPrint("Enter Password");
//       lcdSetCursor(1, 0);
//       lcdPrint(displayText.c_str());
//       lastDisplayText = displayText;
//     }
   
//     // IF IT IS IN UNLOCK MODE
//   }else if(locked == false && wrongTries != 0){
//     lcdSetCursor(0, 0);
//     lcdPrint("Press Enter");
//     lcdSetCursor(1, 0);
//     lcdPrint("To Lock");

//     int key = readTTP229TouchKey();
//     if (key > 0 && !keyPressed[key - 1]) {
//       processKeyPress(key);
//       keyPressed[key - 1] = true;
//       delayMs(300);
//     } else if (key == 0) {
//       for (int i = 0; i < 16; i++) keyPressed[i] = false;
//     }

//     if (key == KEY_ACCEPT){
//       lcdClear();
//       lcdSetCursor(0,0);
//       lcdPrint("Locked");
//       setMotorDirection(90);
//       locked = true;
//       delayMs(500);
//       clearInput();
//     }


//     // IF IN SHUTDOWN MODE
//   }else{

//     lcdSetCursor(0, 0);
//     lcdPrint("You're not the");
//     lcdSetCursor(1, 0);
//     lcdPrint("OWNER!!");
//     delayMs(1000);
    
//     for (int i = 0; i < 10; i++) { //stuck for 3 minutes, for now just 10 sec
//       if(i <= 5){ // turn buzzer for 5 sec
//         chirpBuzzer(0.9);
//       }

//       lcdSetCursor(0, 0);
//       lcdPrint("Please wait for");
//       String waitMsg = String(10 - i) + " Sec";
//       lcdSetCursor(1, 0);
//       lcdPrint(waitMsg.c_str());
//       delayMs(1000); // delay 1 second
//       lcdClear();
//     }

//     wrongTries = 3;
//     turnOffBuzzer();
//     lcdClear();



//   }







//   }
//   return 0;
// }




// // keypad lcd main testing
// // Global tracking variables
// extern String displayText;
// String lastDisplayText = "";
// bool keyPressed[16] = {false};

// int main() {
//   initTimer1();
//   initTimer0();
//   initPWMTimer3();
//   sei();
//   lcdInit();             
//   delayMs(1000);
//   lcdBacklight(true);  
//   Serial.begin(9600);
//   Serial.print("im here ");
  
//   // Initialize Serial and Keypad (using direct register method in initKeypad)
//   initKeypad();
//   Serial.print("im here also ");

//   while (true) {
//     int key = readTTP229TouchKey();
//     if (key > 0 && !keyPressed[key - 1]) {
//       processKeyPress(key);
//       keyPressed[key - 1] = true;
//       delayMs(300);
//     } else if (key == 0) {
//       for (int i = 0; i < 16; i++) keyPressed[i] = false;
//     }

//     if (displayText != lastDisplayText) {
//       Serial.print("Input: ");
//       Serial.println(displayText);
//       lcdClear();
//       lcdSetCursor(0, 0);
//       lcdPrint(displayText.c_str());
//       lastDisplayText = displayText;
//     }
//   }

//   return 0;
// }



// individual tessting main
// int main(){
//   init();
//   initMotorPins();
//   initPWMTimer3();   // Initialize Timer1 for PWM on Pin 9 (OC1A)
//   initTimer0();      // Initialize Timer0 for delayMs
//   sei();             // Enable global interrupts
//   Serial.begin(9600);
  
//   while(1){
//     Serial.print("im here ");
//     setMotorDirection(-90);  // Set motor to -90 degrees
//     delayMs(1000);           // Wait for 1 second
//     setMotorDirection(90);   // Set motor to 90 degrees
//     delayMs(1000);           // Wait for 1 second


//   };

//   return 0;
// }


// individual tessting buzzer
extern String displayText;
String lastDisplayText = "";
bool keyPressed[16] = {false};
bool enteringPasswordMode = false;
int main(){
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
  Serial.begin(9600);
  Serial.print("Finished initializing no probs \n");

  while (1) {
      // lcdPrint("Hello");
      setMotorDirection(90);
      
      chirpBuzzer(0.9); // chirp with 50% duty
      delayMs(1000);    // wait 1 second
      setMotorDirection(-90);
      delayMs(1000);    // wait 1 second
      int key = readTTP229TouchKey();
      if (key > 0 && !keyPressed[key - 1]) {
        processKeyPress(key);
        keyPressed[key - 1] = true;
        delayMs(300);
      } else if (key == 0) {
        for (int i = 0; i < 16; i++) keyPressed[i] = false;
      }

      if (key > 0 && !keyPressed[key - 1]) {
        processKeyPress(key);
        keyPressed[key - 1] = true;
        delayMs(300);
      } else if (key == 0) {
        for (int i = 0; i < 16; i++) keyPressed[i] = false;
      }

      String msg = String(key);
      lcdClear();
      lcdPrint(msg.c_str());
      // Serial.print(msg);
  }

  return 0;
}
  