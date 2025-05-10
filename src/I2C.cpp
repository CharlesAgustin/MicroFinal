#include "I2C.h"
#define LCD_ADDR 0x27
#define LCD_BACKLIGHT 0x08

uint8_t _backlightVal = LCD_BACKLIGHT;

void InitI2CPin()
{
  TWSR = 0x00;        // Prescaler = 1
  TWBR = 72;          // SCL freq = ~100kHz if F_CPU = 16MHz
  TWCR = (1 << TWEN); // Enable TWI
}

void StartI2CTrans(uint8_t address)
{
  // Send START condition
  TWCR = (1 << TWSTA) | (1 << TWEN) | (1 << TWINT);
  while (!(TWCR & (1 << TWINT)))
    ; // Wait until the start condition is transmitted

  // Send address with write (0) or read (1)
  TWDR = address << 1;               // Left shift the address by 1 to fit the R/W bit
  TWCR = (1 << TWEN) | (1 << TWINT); // Send the address byte
  while (!(TWCR & (1 << TWINT)))
    ; // Wait for transmission to finish
}

void StopI2CTrans()
{
  // Send STOP condition
  TWCR = (1 << TWSTO) | (1 << TWINT) | (1 << TWEN);
  while (TWCR & (1 << TWSTO))
    ; // Wait until the STOP condition is finished
}

void Write(uint8_t data)
{
  TWDR = data;                       // set the data to TWDR
  TWCR = (1 << TWEN) | (1 << TWINT); // send the data
  while (!(TWCR & (1 << TWINT)))
    ; // Wait until the STOP condition is finished
}

void SendI2C(uint8_t value)
{
  StartI2CTrans(LCD_ADDR);      // start transmission to LCD
  Write(value | _backlightVal); // write to the LCD to turn it on
  StopI2CTrans();               // stop the transmission after done
}
