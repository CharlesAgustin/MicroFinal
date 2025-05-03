#ifndef MOTOR_H
#define MOTOR_H
#include <avr/io.h>
#include "PWM.h"

void initMotorPins();
void setMotorDirection(unsigned int direction);
#endif