#include "microDS3231.h"
#include <Arduino.h>
#include <microWire.h>
#include "buildTime.h"

// взято из RTClib
const uint8_t daysInMonth [] PROGMEM = { 31, 28, 31, 30, 31, 30, 31, 31, 30, 31, 30, 31 };
static uint16_t getWeekDay(uint16_t y, uint8_t m, uint8_t d) {
  if (y >= 2000)
    y -= 2000;
  uint16_t days = d;
  for (uint8_t i = 1; i < m; ++i)
    days += pgm_read_byte(daysInMonth + i - 1);
  if (m > 2 && y % 4 == 0)
    ++days;
  //return days + 365 * y + (y + 3) / 4 - 1;
  return (days + 365 * y + (y + 3) / 4 - 1 + 6) % 7;
}

// поiхали
MicroDS3231::MicroDS3231(void) {
	Wire.begin();
}

void MicroDS3231::setTime(uint8_t seconds , uint8_t minutes , uint8_t hours , uint8_t date, uint8_t month, uint16_t year) {
	uint8_t day = getWeekDay(year, month, date);
	year -= 2000;
	Wire.beginTransmission(0x68);
	Wire.write(0x00);
	Wire.write((((seconds / 10) << 4) | (seconds % 10))); // пишем по очереди
	Wire.write((((minutes / 10) << 4) | (minutes % 10)));
	if (hours > 19) {
		Wire.write((0x2 << 4) | (hours % 20));
	}
	else if (hours > 9) {
		Wire.write((0x1 << 4) | (hours % 10));
	}
	else {
		Wire.write(hours);
	}
	Wire.write(day);
	Wire.write((((date / 10) << 4) | (date % 10)));
	Wire.write((((month / 10) << 4) | (month % 10)));
	Wire.write((((year / 10) << 4) | (year % 10)));
	Wire.endTransmission();
}

void MicroDS3231::setTime(uint8_t param) {
	if (param) MicroDS3231::setTime(BUILD_SEC, BUILD_MIN, BUILD_HOUR, BUILD_DAY, BUILD_MONTH, BUILD_YEAR);
}

bool MicroDS3231::lostPower(void) { // возвращает true если 1 января 2000
	if(MicroDS3231::getYear() == 2000 && MicroDS3231::getMonth() == 1 && MicroDS3231::getDate() == 1) return true; 
	else return false;
}


uint8_t MicroDS3231::readRegister(uint8_t addr) {
	Wire.beginTransmission(0x68); 
	Wire.write(addr);
	Wire.beginTransmission();
	Wire.write(0xD1);
	uint8_t data = Wire.receive_nack();
	Wire.endTransmission();
	return data;
}

uint8_t MicroDS3231::getSeconds(void) {
	uint8_t r_data = MicroDS3231::readRegister(0x00); // читаем регистр
	return  (uint8_t)(((r_data & 0x70) * 0.625) + (r_data & 0xF)); // дербаним байт и возвращаем
}

uint8_t MicroDS3231::getMinutes(void) {
	uint8_t r_data = MicroDS3231::readRegister(0x01);
	return  (uint8_t)(((r_data & 0x70)  * 0.625) + (r_data & 0xF));
}

uint8_t MicroDS3231::getHours(void) { 
	uint8_t r_data = MicroDS3231::readRegister(0x02);
	if (r_data & 0x20) return (uint8_t)((r_data & 0xF) + 20);
	else if (r_data & 0x10) return (uint8_t)((r_data & 0xF) + 10);
	else return (uint8_t)(r_data & 0xF);
}

uint8_t MicroDS3231::getDay(void) {
	return MicroDS3231::readRegister(0x03);
}

uint8_t MicroDS3231::getDate(void) { 
	uint8_t r_data = MicroDS3231::readRegister(0x04);
	return  (uint8_t)(((r_data & 0x30)  * 0.625) + (r_data & 0xF));
}

uint8_t MicroDS3231::getMonth(void) { 
	uint8_t r_data = MicroDS3231::readRegister(0x05);
	return  (uint8_t)(((r_data & 0x10)  * 0.625) + (r_data & 0xF));
}

uint16_t MicroDS3231::getYear(void) { 
	uint8_t r_data = MicroDS3231::readRegister(0x06);
	return  (uint16_t)(((r_data & 0xF0)  * 0.625) + (r_data & 0xF) + 2000);
}