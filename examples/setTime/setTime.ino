/*
 * 
 * setTime.ino
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

  Serial.println("What is the current second?");
  while(!Serial.available()){} // Wait for user input
  rtc.setSecond(Serial.parseInt()); // Set the current second onto the chip

  Serial.println("What is the current minute?");
  while(!Serial.available()){} // Wait for user input
  rtc.setMinute(Serial.parseInt()); // Set the current minute onto the chip

  Serial.println("What is the current hour? (in 24-hour format)");
  while(!Serial.available()){} // Wait for user input
  rtc.setHour(Serial.parseInt()); // Set the current hour onto the chip

  Serial.println("What is the current day?");
  while(!Serial.available()){} // Wait for user input
  rtc.setDay(Serial.parseInt()); // Set the current day onto the chip

  Serial.println("What is the current month?");
  while(!Serial.available()){} // Wait for user input
  rtc.setMonth(Serial.parseInt()); // Set the current month onto the chip

  Serial.println("What is the current year?");
  while(!Serial.available()){} // Wait for user input
  rtc.setYear(Serial.parseInt()); // Set the current year onto the chip

  rtc.setWeekday(weekdayFromDate(rtc.getDay(), rtc.getMonth(), rtc.getYear())); // Automatically set the weekday using weekdayFromDate()
}

void loop()
{
  Serial.println("The current date and time is");
  Serial.print(valueToWeekday(rtc.getWeekday())); // Gets the weekday number, then converts it to a string and prints it
  Serial.print(", ");
  Serial.println(rtc.getDateTime(FORMAT_MDY, FORMAT_12HR)); // Print date and time in month/day/year format and 12-hour format
  delay(5000);
}
