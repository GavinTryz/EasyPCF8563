/*
 * 
 * alarm.ino
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

  // Before enabling an alarm, it's good practice to remove any previous alarms that may have been left on.
  // Do this by disabling the alarms to remove them any active ones, and clear any alarm that may have gone off.
  rtc.alarmDisable();
  rtc.alarmClear();

  // Same thing for any timers
  rtc.timerDisable();
  rtc.timerClear();
  
  // By calling any of the alarm...() functions, you enable the alarm, and set the desired time.
  rtc.alarmMinute(15);
  // Right now, the alarm will go off every hour at the 15th minute
  
  rtc.alarmHour(21);
  // Now, the alarm will go off every day at 21:15 (7:15PM)

  // By using alarmWeekday(), you can specify the alarm to only go off on a certain day. You cannot select multiple days with the PCF8563
  rtc.alarmWeekday(2);
  // Now, the alarm will only go off on Tuesdays at 21:15 (7:15PM)

  // We want to be able to see if the alarm goes off. Using alarmInterrupt() allows us to use the INT pin with the alarm.
  // We can also use rtc.alarmStatus() to check. We use this in the loop() function. 
  rtc.alarmInterrupt(true);
  // INT is "normally open," so when the alarm goes off, the INT pin will allow electricity to flow.
}

void loop()
{
  Serial.print(rtc.getTime(FORMAT_24HR));
  Serial.print(" - ");
  
  if(rtc.alarmStatus())
  {
    Serial.println("Alarm went off! LED should now be on!");
    delay(5000);
    rtc.alarmClear();
    Serial.println("Alarm reset and ready for next occurrence. LED should now be off.");
  }
  else
    Serial.println("Waiting for alarm to go off...");
    
  delay(1000);
}
