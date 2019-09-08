#include <microDS3231.h>
MicroDS3231 rtc;

void setup() {
  uartBegin();

  if (rtc.lostPower()) {  //  при потере питания
    rtc.setTime(COMPILE_TIME);  // установить время компиляции
  }
  uartPrintln(rtc.getHours());
  uartPrintln(rtc.getMinutes());
  uartPrintln(rtc.getSeconds());
  uartPrintln(rtc.getDate());
  uartPrintln(rtc.getMonth());
  uartPrintln(rtc.getYear());
}

void loop() {
}
// 2466 (8%)
// 75 (3%)
