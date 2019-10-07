/*
 * 
 * timer.ino
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
 * Then connect 5V from the Arduino to a 220+Ω resistor
 * and connect that to an LED's anode (+). Connect the
 * cathode (-) to INT pin on the PCF8563.
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

  // Before starting a timer, it's a good idea to be safe by stopping and clearing any previous timer that may have been left on.
  rtc.timerDisable();
  rtc.timerClear();

  // Same thing for any alarms
  rtc.alarmDisable();
  rtc.alarmClear();

  // Set the value for the timer to 15 seconds
  rtc.timerSet(15);
  
  // Let's set our timer multiplier. The value will be in seconds if we set it to "2". We can also set 1/4096th, 1/64th seconds as well as minutes.
  rtc.timerMultiplier(2);
  // 0 = 1/4096, 1 = 1/64, 2 = 1, 3 = 60 (seconds, 3 only goes off at the 0 second of the current minute)

  // We want to be able to see if the timer goes off. Using timerInterrupt() allows us to use the INT pin with the timer.
  // We can also use rtc.timerStatus() to check. We use this in the loop() function.
  rtc.timerInterrupt(true);
  // INT is "normally open," so when the alarm goes off, the INT pin will allow electricity to flow.

  // Now that the timer is all set up and ready, let's start it
  rtc.timerEnable();
}

void loop()
{
  Serial.print(rtc.getTime(FORMAT_24HR));
  Serial.print(" - ");
  Serial.print("Time remaining: ");
  Serial.println(rtc.timerRemaining());

  // As soon as the timer passes 1 (and resets to the input)...
  if(rtc.timerStatus())
  {
    Serial.println("Timer went off! LED should now be on!");
    
    // If we didn't disable the timer before the delay, the timer would restart and run while the delay leaves the LED on.
    rtc.timerDisable();
    delay(5000);
    
    Serial.println("Timer reset and ready to count down again. LED should now be off.");
    
    // Clearing the timer allows it to go off again.
    rtc.timerClear();
    rtc.timerEnable();
  }
  
  delay(1000);
}
