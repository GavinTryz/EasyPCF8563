/*
 * 
 *  EasyPCF8563 - v1.0
 *  Code by Gavin Tryzbiak
 *  https://github.com/GavinTryz
 *  
 *  Arduino library built to communicate with the PCF8563 Real Time Clock chip via I2C.
 *  
 */

#include "EasyPCF8563.h"

// Constructors ///////////////////////////////////////////////////////////////////////

EasyPCF8563::EasyPCF8563(void) // No specified address (assume DEFAULT_SLAVE_ADR)
{
  Wire.begin();
  _slaveAdr = DEFAULT_SLAVE_ADR;
}

EasyPCF8563::EasyPCF8563(uint8_t addressInput)
{
  Wire.begin();
  _slaveAdr = addressInput;
}

// Public Members /////////////////////////////////////////////////////////////////////

// Data retrieval
bool EasyPCF8563::getVoltageStatus(void)
{
  return _readFromAddress(ADR_SECONDS) >> 7;
}

uint8_t EasyPCF8563::getSecond(void)
{
  return _BCDToDec(_readFromAddress(ADR_SECONDS) & 0b01111111);
}

uint8_t EasyPCF8563::getMinute(void)
{
  return _BCDToDec(_readFromAddress(ADR_MINUTES) & 0b01111111);
}

uint8_t EasyPCF8563::getHour(void)
{
  return _BCDToDec(_readFromAddress(ADR_HOURS) & 0b00111111);
}

uint8_t EasyPCF8563::getDay(void)
{
  return _BCDToDec(_readFromAddress(ADR_DAYS) & 0b00111111);
}

uint8_t EasyPCF8563::getWeekday(void)
{
  return _BCDToDec(_readFromAddress(ADR_WEEKDAYS) & 0b00000111);
}

uint8_t EasyPCF8563::getMonth(void)
{
  return _BCDToDec(_readFromAddress(ADR_MONTHS) & 0b00011111);
}

uint16_t EasyPCF8563::getYear(void)
{
  return 2000 + (((_readFromAddress(ADR_MONTHS) >> 7) & 0b00000001) * 100) + _BCDToDec(_readFromAddress(ADR_YEARS));
}

String EasyPCF8563::getTime(uint8_t timeFormat)
{
  String result = "";
  uint8_t hour = getHour();
  uint8_t minute = getMinute();
  uint8_t second = getSecond();
  
  if(timeFormat == FORMAT_12HR)
  {
    uint8_t hour = getHour();
    result += (hour == 0 ? 12 : hour % 12); // If hour is 0 use 12 instead
    result += ':';
    if(minute < 10)
      result += '0';
    result += minute;
    result += ':';
    if(second < 10)
      result += '0';
    result += second;
    result += (hour < 12 ? "AM" : "PM");
  }
  else // timeFormat == FORMAT_24HR
  {
    if(hour < 10)
      result += '0';
    result += hour;
    result += ':';
    if(minute < 10)
      result += '0';
    result += minute;
    result += ':';
    if(second < 10)
      result += '0';
    result += second;
  }
  return result;
}

String EasyPCF8563::getDate(uint8_t dateFormat)
{
  String result = "";
  if(dateFormat == FORMAT_DMY)
  {
    result += getDay();
    result += '/';
    result += getMonth();
    result += '/';
    result += getYear();
  }
  else if(dateFormat == FORMAT_MDY)
  {
    result += getMonth();
    result += '/';
    result += getDay();
    result += '/';
    result += getYear();
  }
  else // dateFormat == FORMAT_YMD
  {
    result += getYear();
    result += '/';
    result += getMonth();
    result += '/';
    result += getDay();
  }
  return result;
}

String EasyPCF8563::getDateTime(uint8_t format1, uint8_t format2)
{
  String result = "";
  if(format1 > 3) // If format1 is a time
    result += getTime(format1);
  else
    result += getDate(format1);
    
  result += ' ';
  
  if(format2 > 3) // If format2 is a time
    result += getTime(format2);
  else
    result += getDate(format2);
    
  return result;
}

// Data setting
void EasyPCF8563::setSecond(uint8_t second)
{
  _writeToAddress(ADR_SECONDS, _decToBCD(second));
}

void EasyPCF8563::setMinute(uint8_t minute)
{
  _writeToAddress(ADR_MINUTES, _decToBCD(minute));
}

void EasyPCF8563::setHour(uint8_t hour)
{
  _writeToAddress(ADR_HOURS, _decToBCD(hour));
}

void EasyPCF8563::setDay(uint8_t day)
{
  _writeToAddress(ADR_DAYS, _decToBCD(day));
}

void EasyPCF8563::setWeekday(uint8_t weekday)
{
  _writeToAddress(ADR_WEEKDAYS, weekday);
}

void EasyPCF8563::setMonth(uint8_t month)
{
  _writeToAddress(ADR_MONTHS, (_readFromAddress(ADR_MONTHS) & 0b10000000) | _decToBCD(month));
}

void EasyPCF8563::setYear(uint16_t year)
{
  if(year - 2000 > 99)
    _writeToAddress(ADR_MONTHS, 0b10000000 | (_readFromAddress(ADR_MONTHS) & 0b01111111)); // Set century to 1
  else
    _writeToAddress(ADR_MONTHS,_readFromAddress(ADR_MONTHS) & 0b01111111); // Set century to 0
  _writeToAddress(ADR_YEARS, _decToBCD(year % 100));
}

void EasyPCF8563::autoSync() // Must be called in setup()
{
  char compileMonth[4];
  uint16_t compileHour, compileMinute, compileSecond, compileDay, compileYear;
  // __DATE__ and __TIME__ are compile time macros. Note that these are the date and time from when the sketch is COMPILED. Not uploaded. May be off by a few seconds
  // This should NOT be included on a sketch with other uses. If board power is reset, this function will reset the chip back to the date and time the sketch was compiled
  sscanf(__TIME__, "%d:%d:%d", &compileHour, &compileMinute, &compileSecond);
  sscanf(__DATE__, "%s %d %d", compileMonth, &compileDay, &compileYear);
  setSecond(compileSecond);
  setMinute(compileMinute);
  setHour(compileHour);
  setDay(compileDay);
  setWeekday(weekdayFromDate(compileDay, shortMonthToValue(compileMonth), compileYear));
  setMonth(shortMonthToValue(compileMonth));
  setYear(compileYear);
}

// Alarm
void EasyPCF8563::alarmMinute(uint8_t minute)
{
  _writeToAddress(ADR_ALARM_MINUTE, 0b01111111 & _decToBCD(minute));
}

void EasyPCF8563::alarmHour(uint8_t hour)
{
  _writeToAddress(ADR_ALARM_HOUR, 0b01111111 & _decToBCD(hour));
}

void EasyPCF8563::alarmDay(uint8_t day)
{
  _writeToAddress(ADR_ALARM_DAY, 0b01111111 & _decToBCD(day));
}

void EasyPCF8563::alarmWeekday(uint8_t weekday)
{
  _writeToAddress(ADR_ALARM_WEEKDAY, 0b01111111 & weekday);
}

void EasyPCF8563::alarmInterrupt(bool value)
{
  if(value)
    _writeToAddress(ADR_CONTROL_STATUS_2, _readFromAddress(ADR_CONTROL_STATUS_2) | 0b00000010);
  else
    _writeToAddress(ADR_CONTROL_STATUS_2, _readFromAddress(ADR_CONTROL_STATUS_2) & 0b11111101);
}

void EasyPCF8563::alarmClear(void)
{
  _writeToAddress(ADR_CONTROL_STATUS_2, _readFromAddress(ADR_CONTROL_STATUS_2) & 0b11110111);
}

void EasyPCF8563::alarmDisable(void)
{
  _writeToAddress(ADR_ALARM_MINUTE, 0b10000000);
  _writeToAddress(ADR_ALARM_HOUR, 0b10000000);
  _writeToAddress(ADR_ALARM_DAY, 0b10000000);
  _writeToAddress(ADR_ALARM_WEEKDAY, 0b10000000);
}

// Timer
void EasyPCF8563::timer(uint8_t value)
{
  _writeToAddress(ADR_TIMER, value);
}

void EasyPCF8563::timerMultiplier(uint8_t value)
{
  _writeToAddress(ADR_TIMER_CONTROL, (_readFromAddress(ADR_TIMER_CONTROL) & 0b10000000) | (value & 0b00000011));
}

void EasyPCF8563::timerInterrupt(bool value)
{
  if(value)
    _writeToAddress(ADR_CONTROL_STATUS_2, _readFromAddress(ADR_CONTROL_STATUS_2) | 0b00000001);
  else
    _writeToAddress(ADR_CONTROL_STATUS_2, _readFromAddress(ADR_CONTROL_STATUS_2) & 0b11111110);
}

void EasyPCF8563::timerPulse(bool value)
{
  if(value)
    _writeToAddress(ADR_CONTROL_STATUS_2, _readFromAddress(ADR_CONTROL_STATUS_2) | 0b00010000);
  else
    _writeToAddress(ADR_CONTROL_STATUS_2, _readFromAddress(ADR_CONTROL_STATUS_2) & 0b11101111);
}

void EasyPCF8563::timerEnable(void)
{
  _writeToAddress(ADR_TIMER_CONTROL, _readFromAddress(ADR_TIMER_CONTROL) | 0b10000000);
}

void EasyPCF8563::timerClear(void)
{
  _writeToAddress(ADR_CONTROL_STATUS_2, _readFromAddress(ADR_CONTROL_STATUS_2) & 0b11111011);
}

void EasyPCF8563::timerDisable(void)
{
  _writeToAddress(ADR_TIMER_CONTROL, _readFromAddress(ADR_TIMER_CONTROL) & 0b01111111);
}

// Private Members ////////////////////////////////////////////////////////////////////
void EasyPCF8563::_writeToAddress(uint16_t writeAddress, uint8_t data)
{
  Wire.beginTransmission(_slaveAdr);
  Wire.write(writeAddress);
  Wire.write(data);
  Wire.endTransmission();
}

uint8_t EasyPCF8563::_readFromAddress(uint16_t address)
{
  Wire.beginTransmission(_slaveAdr);
  Wire.write(address);
  Wire.endTransmission();
  Wire.requestFrom(_slaveAdr, 1);
  return Wire.read();
}

uint8_t EasyPCF8563::_decToBCD(uint8_t value)
{
  return ((value / 10 * 16) + (value % 10));
}

uint8_t EasyPCF8563::_BCDToDec(uint8_t value)
{
  return ((value / 16 * 10) + (value % 16));
}

// Standard Functions /////////////////////////////////////////////////////////////////

uint8_t weekdayFromDate(uint8_t day, uint8_t month, uint16_t year) // Calculate weekday from date using Zeller's Rule
{
  return (day + (((13 * (((month+9) % 12) + 1)) - 1) / 5) + ((month < 3 ? year - 1 : year)%100) + ((year%100)/4) + (((month < 3 ? year - 1 : year)/100)/4) - (2*(year/100))) % 7;
}

uint8_t shortMonthToValue(String s)
{
  if(s.equals("Jan"))
    return 1;
  else if(s.equalsIgnoreCase("Feb"))
    return 2;
  else if(s.equalsIgnoreCase("Mar"))
    return 3;
  else if(s.equalsIgnoreCase("Apr"))
    return 4;
  else if(s.equalsIgnoreCase("May"))
    return 5;
  else if(s.equalsIgnoreCase("Jun"))
    return 6;
  else if(s.equalsIgnoreCase("Jul"))
    return 7;
  else if(s.equalsIgnoreCase("Aug"))
    return 8;
  else if(s.equalsIgnoreCase("Sep"))
    return 9;
  else if(s.equalsIgnoreCase("Oct"))
    return 10;
  else if(s.equalsIgnoreCase("Nov"))
    return 11;
  else if(s.equalsIgnoreCase("Dec"))
    return 12;
  else
    return 0;
}

String valueToMonth(uint8_t value)
{
  switch(value)
  {
  case 1:
    return "January";
  case 2:
    return "February";
  case 3:
    return "March";
  case 4:
    return "April";
  case 5:
    return "May";
  case 6:
    return "June";
  case 7:
    return "July";
  case 8:
    return "August";
  case 9:
    return "September";
  case 10:
    return "October";
  case 11:
    return "November";
  case 12:
    return "December";
  default:
    return "UnknownMonth";    
  }
}

String valueToShortMonth(uint8_t value)
{
  switch(value)
  {
  case 1:
    return "Jan";
  case 2:
    return "Feb";
  case 3:
    return "Mar";
  case 4:
    return "Apr";
  case 5:
    return "May";
  case 6:
    return "Jun";
  case 7:
    return "Jul";
  case 8:
    return "Aug";
  case 9:
    return "Sep";
  case 10:
    return "Oct";
  case 11:
    return "Nov";
  case 12:
    return "Dec";
  default:
    return "Unk";
  }
}

String valueToWeekday(uint8_t value)
{
  switch(value)
  {
  case 0:
    return "Sunday";
  case 1:
    return "Monday";
  case 2:
    return "Tuesday";
  case 3:
    return "Wednesday";
  case 4:
    return "Thursday";
  case 5:
    return "Friday";
  case 6:
    return "Saturday";
  default:
    return "UnknownWeekday";
  }
}

String valueToShortWeekday(uint8_t value)
{
  switch(value)
  {
  case 0:
    return "Sun";
  case 1:
    return "Mon";
  case 2:
    return "Tue";
  case 3:
    return "Wed";
  case 4:
    return "Thu";
  case 5:
    return "Fri";
  case 6:
    return "Sat";
  default:
    return "Unk";
  }
}
