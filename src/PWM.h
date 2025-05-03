#ifndef PWM_H
#define PWM_H
#include <avr/io.h>

void initBuzzerPin(); // OCR3A
void initMotorPin();  // OCR3B
void initPWMTimer3();
void changeDutyCycle(unsigned int cycle);
void turnOnBuzzerOn();
void turnOffBuzzer();
void turnOnMotor();
void turnOffMotor();
void chirpBuzzer(float duty);

#endif
