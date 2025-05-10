#ifndef PWM_H
#define PWM_H
#include <avr/io.h>

void initBuzzerPin();
void initPWMTimer2();
void changeDutyCycle(float duty);
void turnOffBuzzer();
void chirpBuzzer(float duty);
void initMotorPins();
void initPWMTimer3();
void setMotorDirection(unsigned int angle);
#endif
