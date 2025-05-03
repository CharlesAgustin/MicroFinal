#include <Arduino.h>
#include <avr/interrupt.h>
#include "buzzer.h"
#include "I2C.h"
#include "keypad.h"
#include "LCD.h"
#include "motor.h"
#include "PWM.h"
#include "timer.h"

// needs to setup reset interrupt too