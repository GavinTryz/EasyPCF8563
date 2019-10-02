/*
 * 
 *  EasyPCF8563 - v1.0
 *  Code by Gavin Tryzbiak
 *  https://github.com/GavinTryz
 *  
 *  Arduino library built to communicate with the PCF8563 Real Time Clock chip via I2C.
 *  
 */

#ifndef EasyPCF8563_h
#define EasyPCF8563_h

#include <Arduino.h>
#include <Wire.h>

#define FORMAT_DMY 0
#define FORMAT_MDY 1
#define FORMAT_YMD 2
#define FORMAT_12HR 12
#define FORMAT_24HR 24
#define SUNDAY 0
#define MONDAY 1
#define TUESDAY 2
#define WEDNESDAY 3
#define THURSDAY 4
#define FRIDAY 5
#define SATURDAY 6

#define DEFAULT_SLAVE_ADR 0x51
#define ADR_CONTROL_STATUS_2 0x01
#define ADR_SECONDS 0x02
#define ADR_MINUTES 0x03
#define ADR_HOURS 0x04
#define ADR_DAYS 0x05
#define ADR_WEEKDAYS 0x06
#define ADR_MONTHS 0x07
#define ADR_YEARS 0x08
#define ADR_ALARM_MINUTE 0x09
#define ADR_ALARM_HOUR 0x0A
#define ADR_ALARM_DAY 0x0B
#define ADR_ALARM_WEEKDAY 0x0C
#define ADR_CLKOUT_CONTROL 0x0D
#define ADR_TIMER_CONTROL 0x0E
#define ADR_TIMER 0x0F

class EasyPCF8563
{
  public:
    // Constructors
    EasyPCF8563(void); // No specified address (assume 0x51)
    EasyPCF8563(uint8_t addressInput); // Specified I2C address

    // Members
    bool getVoltageStatus(void);
    uint8_t getSecond(void);
    uint8_t getMinute(void);
    uint8_t getHour(void);
    uint8_t getDay(void);
    uint8_t getWeekday(void);
    uint8_t getMonth(void);
    uint16_t getYear(void);
    String getTime(uint8_t timeFormat);
    String getDate(uint8_t dateFormat);
    String getDateTime(uint8_t format1, uint8_t format2);

    void setSecond(uint8_t second);
    void setMinute(uint8_t minute);
    void setHour(uint8_t hour);
    void setDay(uint8_t day);
    void setWeekday(uint8_t weekday);
    void setMonth(uint8_t month);
    void setYear(uint16_t year);
    void autoSync(void);
    
    void alarmMinute(uint8_t minute);
    void alarmHour(uint8_t hour);
    void alarmDay(uint8_t day);
    void alarmWeekday(uint8_t weekday);
    void alarmInterrupt(bool value);
    void alarmClear(void);
    void alarmDisable(void);
    
    void timer(uint8_t value);
    void timerMultiplier(uint8_t value); // 0 = 1/4096, 1 = 1/64, 2 = 1, 3 = 60 (seconds, 3 may be unstable)
    void timerInterrupt(bool value);
    void timerPulse(bool value);
    void timerEnable(void);
    void timerClear(void);
    void timerDisable(void);
    
    
  private:
    uint8_t _slaveAdr;

    void _writeToAddress(uint16_t startAddress, uint8_t data);
    uint8_t _readFromAddress(uint16_t address);
    uint8_t _decToBCD(uint8_t value);
    uint8_t _BCDToDec(uint8_t value);
};

uint8_t weekdayFromDate(uint8_t day, uint8_t month, uint16_t year);
uint8_t shortMonthToValue(String s);
String valueToMonth(uint8_t value);
String valueToShortMonth(uint8_t value);
String valueToWeekday(uint8_t value);
String valueToShortWeekday(uint8_t value);

#endif
