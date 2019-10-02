/*
 * 
 * getTime.ino
 * Example sketch for the EasyPCF8563 library
 * 
 * Code by Gavin Tryzbiak
 * https://github.com/GavinTryz
 *  
 */

/*
 * 
 * To run this eample, connect...
 * +---------------------+
 * | PCF8563     Arduino |
 * +---------------------+
 * |   VCC   -->   5V    |
 * |   GND   -->   GND   |
 * |   SDA   -->   A4    |
 * |   SCL   -->   A5    |
 * +---------------------+
 * 
 */

#include <EasyPCF8563.h>
#include <Wire.h>

// Create an instance of the real time clock, at address 0x51
// If it doesn't work and you're unsure what address to use, run an I2C scanner sketch.
EasyPCF8563 rtc(0x51); 

void setup()
{
  Serial.begin(9600);

  Serial.println("[Individual Functions]");
  
  Serial.print("Voltage Status (0 is good): ");
  Serial.println(rtc.getVoltageStatus());
  
  Serial.print("Second: ");
  Serial.println(rtc.getSecond());

  Serial.print("Minute: ");
  Serial.println(rtc.getMinute());

  Serial.print("Hour: ");
  Serial.println(rtc.getHour());

  Serial.print("Day: ");
  Serial.println(rtc.getDay());

  Serial.print("Month: ");
  Serial.println(rtc.getMonth());

  Serial.print("Year: ");
  Serial.println(rtc.getYear());

  Serial.println();

  Serial.println("[Whole Time or Date]");

  Serial.println("Time formats:");
  Serial.println(rtc.getTime(FORMAT_12HR));
  Serial.println(rtc.getTime(FORMAT_24HR));

  Serial.println("Date formats:");
  Serial.println(rtc.getDate(FORMAT_DMY));
  Serial.println(rtc.getDate(FORMAT_MDY));
  Serial.println(rtc.getDate(FORMAT_YMD));

  Serial.println();

  Serial.println("[Combined Date and Time]");

  Serial.println(rtc.getDateTime(FORMAT_12HR, FORMAT_MDY));
  Serial.println(rtc.getDateTime(FORMAT_YMD, FORMAT_24HR));
  Serial.println("You can use any combination of time and date formats with getDateTime()!");
}

void loop()
{
  delay(5000);
}
