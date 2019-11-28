// Wire Master Writer
// by Nicholas Zambetti <http://www.zambetti.com>

// Demonstrates use of the Wire library
// Writes data to an I2C/TWI slave device
// Refer to the "Wire Slave Receiver" example for use with this

// Created 29 March 2006

// This example code is in the public domain.

int pin =7;
#include <Wire.h>

void setup()
{
  pinMode(pin,INPUT);
  Wire.begin(); // join i2c bus (address optional for master)
}

byte x = 0;

void loop()
{
  int u = digitalRead(pin);
  Wire.beginTransmission(4); // transmit to device #4
  if(u == LOW)
  {
  Wire.write("x is ");        // sends five bytes
  Wire.write(x);              // sends one byte  
  Wire.endTransmission();    // stop transmitting

  x++;
  }
  delay(500);
}
