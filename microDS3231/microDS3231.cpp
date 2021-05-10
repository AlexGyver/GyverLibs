#include "microDS3231.h"

// взято из RTClib
static const uint8_t daysInMonth [] PROGMEM = { 31, 28, 31, 30, 31, 30, 31, 31, 30, 31, 30, 31 };

static uint16_t getWeekDay(uint16_t y, uint8_t m, uint8_t d) {
	if (y >= 2000)
	y -= 2000;
	uint16_t days = d;
	for (uint8_t i = 1; i < m; ++i)
	days += pgm_read_byte(daysInMonth + i - 1);
	if (m > 2 && y % 4 == 0)
	++days;
	return (days + 365 * y + (y + 3) / 4 + 4) % 7 + 1;
}

// поiхали
MicroDS3231::MicroDS3231(void) {
	Wire.begin();
}

void MicroDS3231::setTime(int8_t seconds, int8_t minutes, int8_t hours, int8_t date, int8_t month, int16_t year) {
	// защиты от дурака
	month = constrain(month, 1, 12);
	date = constrain(date, 0, pgm_read_byte(daysInMonth + month - 1));
	seconds = constrain(seconds, 0, 59);
	minutes = constrain(minutes, 0, 59);
	hours = constrain(hours, 0, 23);
	
	// отправляем
	uint8_t day = getWeekDay(year, month, date);
	year -= 2000;
	Wire.beginTransmission(DS_ADDR);
	Wire.write(0x00);
	Wire.write(encodeRegister(seconds));
	Wire.write(encodeRegister(minutes));
	if (hours > 19) Wire.write((0x2 << 4) | (hours % 20));
	else if (hours > 9) Wire.write((0x1 << 4) | (hours % 10));
	else Wire.write(hours);	
	Wire.write(day);
	Wire.write(encodeRegister(date));
	Wire.write(encodeRegister(month));
	Wire.write(encodeRegister(year));
	Wire.endTransmission();
}

void MicroDS3231::setTime(DateTime time) {
	MicroDS3231::setTime(time.second, time.minute, time.hour, time.date, time.month, time.year);
}

void MicroDS3231::setTime(uint8_t param) {
	if (param) MicroDS3231::setTime(BUILD_SEC, BUILD_MIN, BUILD_HOUR, BUILD_DAY, BUILD_MONTH, BUILD_YEAR);
}

DateTime MicroDS3231::getTime() {
	DateTime now;
	Wire.beginTransmission(DS_ADDR);
	Wire.write(0x0);
	if (Wire.endTransmission() != 0) return now;
	Wire.requestFrom(DS_ADDR, 7);
	now.second = unpackRegister(Wire.read());
	now.minute = unpackRegister(Wire.read());
	now.hour = unpackHours(Wire.read());
	now.day = Wire.read();
	now.date = unpackRegister(Wire.read());
	now.month = unpackRegister(Wire.read());
	now.year = unpackRegister(Wire.read()) + 2000;
	return now;
}
String MicroDS3231::getTimeString() {
	DateTime now = getTime();
	String str = "";
	if (now.hour < 10) str += '0';
	str += now.hour;
	str += ':';
	if (now.minute < 10) str += '0';
	str += now.minute;
	str += ':';
	if (now.second < 10) str += '0';
	str += now.second;
	return str;
}
String MicroDS3231::getDateString() {
	DateTime now = getTime();
	String str = "";
	str += now.year;
	str += '.';
	if (now.month < 10) str += '0';
	str += now.month;
	str += '.';
	if (now.date < 10) str += '0';
	str += now.date;
	return str;
}
void MicroDS3231::getTimeChar(char* array) {
	DateTime now = getTime();
	array[0] = now.hour / 10 + '0';
	array[1] = now.hour % 10 + '0';
	array[2] = ':';
	array[3] = now.minute / 10 + '0';
	array[4] = now.minute % 10 + '0';
	array[5] = ':';
	array[6] = now.second / 10 + '0';
	array[7] = now.second % 10 + '0';
	array[8] = NULL;
}
void MicroDS3231::getDateChar(char* array) {
	DateTime now = getTime();
	itoa(now.year, array, DEC);
	array[4] = '.';
	array[5] = now.month / 10 + '0';
	array[6] = now.month % 10 + '0';
	array[7] = '.';
	array[8] = now.date / 10 + '0';
	array[9] = now.date % 10 + '0';
	array[10] = NULL;
}
bool MicroDS3231::lostPower(void) { // возвращает true, если 1 января 2000
	if (MicroDS3231::getYear() == 2000 && MicroDS3231::getMonth() == 1 && MicroDS3231::getDate() == 1) return true;
	else return false;
}

uint8_t MicroDS3231::getSeconds(void) {
	return (unpackRegister(readRegister(0x00)));
}

uint8_t MicroDS3231::getMinutes(void) {
	return (unpackRegister(readRegister(0x01)));
}

uint8_t MicroDS3231::getHours(void) {
	return (unpackHours(readRegister(0x02)));
}

uint8_t MicroDS3231::getDay(void) {
	return readRegister(0x03);
}

uint8_t MicroDS3231::getDate(void) {
	return (unpackRegister(readRegister(0x04)));
}

uint8_t MicroDS3231::getMonth(void) {
	return (unpackRegister(readRegister(0x05)));
}

uint16_t MicroDS3231::getYear(void) {
	return (unpackRegister(readRegister(0x06)) + 2000);
}

// сервис
uint8_t MicroDS3231::readRegister(uint8_t addr) {
	Wire.beginTransmission(DS_ADDR);
	Wire.write(addr);
	if (Wire.endTransmission() != 0) return 0;
	Wire.requestFrom(DS_ADDR, 1);
	uint8_t data = Wire.read();
	return data;
}

uint8_t MicroDS3231::unpackRegister(uint8_t data) {
	return ((data >> 4) * 10 + (data & 0xF));
}
uint8_t MicroDS3231::encodeRegister(int8_t data) {
	return (((data / 10) << 4) | (data % 10));
}

uint8_t MicroDS3231::unpackHours(uint8_t data) {
	if (data & 0x20) return ((data & 0xF) + 20);
	else if (data & 0x10) return ((data & 0xF) + 10);
	else return (data & 0xF);
}
float MicroDS3231::getTemperatureFloat(void) {
	return (getTemperatureRaw() * 0.25f);
}
int MicroDS3231::getTemperature(void) {
	return (getTemperatureRaw() >> 2);
}
int MicroDS3231::getTemperatureRaw(void) {
	Wire.beginTransmission(DS_ADDR);
	Wire.write(0x11);
    Wire.endTransmission();
    Wire.requestFrom(DS_ADDR, 2);
	return (Wire.read() << 2 | Wire.read() >> 6);
}