#ifndef BUZZER_H
#define BUZZER_H
#include <avr/io.h>

void initBuzzer();    // initialize buzzer
void turnBuzzerOn();  // function to turn on the buzzer
void turnBuzzerOff(); // function to turn off the buzzer
void chirpBuzzer();

#endif
