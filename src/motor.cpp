#include "motor.h"
#include "timer.h"
#include <Arduino.h>

// void initMotorPins(){
//     DDRH |= (1 << PH6);
// };

// void setMotorDirection(unsigned int angle){
//     int pulseWidth;
    
//     if (angle == -90) {
//         pulseWidth = 1000;
//     } else if (angle == 0) {
//         pulseWidth = 1500;
//     } else if (angle == 90) {
//         pulseWidth = 2000;
//     } else {
//         return; // Invalid input — do nothing
//     }
    
//     for (int i = 0; i < 50; i++) {
//         PORTH |= (1 << PH6);         // HIGH
//         delayUs(pulseWidth);
//         PORTH &= ~(1 << PH6);        // LOW
//         delayMs(20);
//     }
    
      
// };