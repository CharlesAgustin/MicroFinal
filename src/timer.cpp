#include "timer.h"

void initTimer0()
{
    TCCR0A &= ~(1 << WGM00); // Set timer to be in CTC mode
    TCCR0A |= (1 << WGM01);
    TCCR0B &= ~(1 << WGM02);
}

void delayMs(unsigned int delay)
{
    OCR0A = 248;                         // set OCR0A to be 248 ((1ms * 16Mhz)/prescaler) - 1
    TCCR0B |= (1 << CS01) | (1 << CS00); // Sets the prescaler to 64

    for (unsigned int i = 0; i < delay; i++)
    {
        TCNT0 = 0; // Reset counter
        while (!(TIFR0 & (1 << OCF0A)))
            ;                  // keep looping until flag is set
        TIFR0 |= (1 << OCF0A); // unset flag
    }

    TCCR0B &= ~((1 << CS01) | (1 << CS00)); //  turn off timer
}

void initTimer1()
{
    // Set CTC mode
    TCCR1A &= ~(1 << WGM10 | 1 << WGM11);
    TCCR1B |= (1 << WGM12);
}

void delayUs(unsigned int delay)
{

    OCR1A = 15;                          // (1us * 16MHz) / 64 - 1 = 15
    TCCR1B |= (1 << CS11) | (1 << CS10); // set prescaler to 64

    for (unsigned int i = 0; i < delay; i++)
    {
        TCNT1 = 0; // Reset counter
        while (!(TIFR1 & (1 << OCF1A)))
            ;                  // keep looping until flag is set
        TIFR1 |= (1 << OCF1A); // unset flag
    }
    TCCR1B &= ~((1 << WGM12) | (1 << CS11) | (1 << CS10)); // turn off timer
}