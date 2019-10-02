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

// -----------------------------------WARNING ABOUT autoSync()!------------------------------------
// ------------------------------------------------------------------------------------------------
// When you click upload, a copy of the computer's time is saved, sent to the Arduino, and then
// saved onto the Arduino. One problem you may run into is that autoSync() runs 3-15 seconds
// behind your computer's time. This is because it takes 3-15 seconds to compile the sketch and
// upload it to the Arduino. Another problem is that if you, for example, compiled your code
// yesterday, and you were to restart your Arduino today, autoSync() would run, and reset the time
// back to the time you last COMPILED (yesterday)! As a general rule of thumb:
//
// ---------------------------ONLY USE autoSync() IN A SKETCH BY ITSELF!---------------------------
//
// It's only for synchronizing the clock once. Never use "autoSync()" inside an actual project!

  rtc.autoSync();
  Serial.println("The PCF8563 has been synchronized with your computer!");
}

void loop()
{
  Serial.println("\n\nCurrently, the PCF8563 reads...");
  Serial.println(rtc.getDateTime(FORMAT_12HR, FORMAT_MDY));
  Serial.println("\nMake sure to read the note in the sketch!");
  Serial.println("autoSync() may work completely differently than you think, and mess up your project!");
  delay(5000);
}
