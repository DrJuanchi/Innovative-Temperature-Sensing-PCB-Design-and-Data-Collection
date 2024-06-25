#include <Wire.h>

#define Addr 0x48 // ADS7830 I2C address 

void setup()
{
  // Initialise I2C communication as Master
  Wire.begin();
  // Initialise serial communication, set baud rate = 9600
  Serial.begin(9600);
  delay(300);
}

void loop()
{
  unsigned int data;

  // Start I2C transmission
  Wire.beginTransmission(Addr);
  // Channel Selection
  Wire.write(0x8C);
  // Stop I2C transmission
  Wire.endTransmission();
  
  // Request 1 byte of data
  Wire.requestFrom(Addr, 1);
  
  // Read 1 byte of data
  if (Wire.available() == 1)
  {
    data = Wire.read();
  }

  // Output data to serial monitor
  Serial.print("Digital value of analog input  : ");
  Serial.println(data);
  delay(300);
}
