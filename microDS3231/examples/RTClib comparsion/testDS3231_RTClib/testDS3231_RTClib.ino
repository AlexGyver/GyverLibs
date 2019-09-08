#include <Wire.h>
#include <RTClib.h>
RTC_DS3231 rtc;
void setup () {
  uartBegin();
  rtc.begin();

  if (rtc.lostPower()) {
    rtc.adjust(DateTime(F(__DATE__), F(__TIME__)));
  }
  DateTime now = rtc.now();
  uartPrintln(now.hour());
  uartPrintln(now.minute());
  uartPrintln(now.second());
  uartPrintln(now.year());
  uartPrintln(now.month());
  uartPrintln(now.day());
}

void loop () {
}
// 3364 (10%)
// 285 (13%)
