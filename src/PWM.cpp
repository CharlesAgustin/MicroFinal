#include "PWM.h"

#include <Arduino.h>
#include "motor.h"
#include "timer.h"

#include "pwm.h"
#include "timer.h"

void initBuzzerPin()
{
    DDRB |= (1 << DDB4); // Set PB4 (Pin 10, OC2A) as output
}

void initPWMTimer2()
{
    // Initialize Fast PWM mode: WGM22:0 = 011 for Fast PWM with TOP = 0xFF
    TCCR2A |= (1 << WGM20) | (1 << WGM21); // Set WGM20 and WGM21 to 1
    TCCR2B &= ~(1 << WGM22);               // Clear WGM22

    // Set non-inverting mode on OC2A (Pin 11)
    TCCR2A |= (1 << COM2A1);
    TCCR2A &= ~(1 << COM2A0);

    // Set initial duty cycle to 0 (silent)
    OCR2A = 0;
    TCCR2A &= ~(1 << COM2A1);
    TCCR2A &= ~(1 << COM2A0);

    // Set prescaler to 64 → good balance for audible tone range
    TCCR2B |= (1 << CS22);                  // CS22 = 1
    TCCR2B &= ~((1 << CS21) | (1 << CS20)); // CS21 = 0, CS20 = 0
}

void changeDutyCycle(float duty)
{
    OCR2A = 255 * duty; // 8-bit PWM, max is 255
}

void turnOffBuzzer()
{
    TCCR2A &= ~((1 << COM2A1) | (1 << COM2A0));
    PORTB &= ~(1 << PB4); // Pin 10 = PB4
}

void chirpBuzzer(float duty)
{
    TCCR2A |= (1 << COM2A1);
    TCCR2A &= ~(1 << COM2A0);

    changeDutyCycle(duty); // Turn on buzzer
    delayMs(100);          // Wait
    changeDutyCycle(0);    // Turn off buzzer
    delayMs(100);          // Wait
}

void initMotorPins()
{
    DDRE |= (1 << PE3); // Set Pin 5 (OC3A / PE3) as output
}

void initPWMTimer3()
{
    // Set Fast PWM Mode 14 (TOP = ICR3)
    TCCR3A = (1 << COM3A1) | (1 << WGM31);              // Clear OC3A on Compare Match, set at BOTTOM
    TCCR3B = (1 << WGM33) | (1 << WGM32) | (1 << CS31); // Prescaler = 8

    ICR3 = 39999; // 20ms period (50Hz) at 16MHz with prescaler 8
    OCR3A = 3000; // Initial pulse = 1.5ms (center position)
}

void setMotorDirection(unsigned int angle) // Determine the pulse width based on the desired angle
{
    int pulseWidth;

    if (angle == -90)
    {
        pulseWidth = 1000; // 1ms
    }
    else if (angle == 0)
    {
        pulseWidth = 1500; // 1.5ms
    }
    else if (angle == 90)
    {
        pulseWidth = 2000; // 2ms
    }
    else
    {
        return; // Invalid input
    }

    // 1 tick = 0.5µs at prescaler 8 → convert µs to ticks
    OCR3A = pulseWidth * 2;
}

void initMotorPins()
{
    DDRB |= (1 << PB5); // Pin 9 (OC1A) as output
}

void initPWMTimer1()
{
    // Set Fast PWM Mode 14 (TOP = ICR1)
    TCCR1A = (1 << COM1A1) | (1 << WGM11);              // Clear OC1A on Compare Match, set at BOTTOM
    TCCR1B = (1 << WGM13) | (1 << WGM12) | (1 << CS11); // Prescaler = 8

    ICR1 = 39999; // TOP = 20ms period (50Hz) at 16MHz with prescaler 8
    OCR1A = 3000; // Initial position: 1.5ms pulse width (center position for servo)
}

void setMotorDirection(unsigned int angle)
{
    int pulseWidth;

    // Map angle to PWM pulse width (1000µs to 2000µs for servo)
    if (angle == -90)
    {
        pulseWidth = 1000; // 1ms pulse
    }
    else if (angle == 0)
    {
        pulseWidth = 1500; // 1.5ms pulse (center)
    }
    else if (angle == 90)
    {
        pulseWidth = 2000; // 2ms pulse
    }
    else
    {
        return; // Invalid input — do nothing
    }

    // Directly convert the pulse width into the corresponding OCR1A value
    // Pulse width is in microseconds (1000µs = 1ms) and OCR1A is in clock ticks (0.5µs per tick)
    // Pulse width in microseconds / 0.5 = number of timer ticks (OCR1A)
    int ticks = pulseWidth * 2; // Convert microseconds to timer ticks (0.5µs per tick)

    // Set OCR1A to the calculated pulse width in timer ticks
    OCR1A = ticks;

    // The PWM signal will automatically be generated with the new pulse width
    // No need for manual toggling or delays anymore
}
