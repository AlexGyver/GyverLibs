#include <microDS3231.h>
MicroDS3231 rtc;

void setup() {
  Serial.begin(9600);

  // получаем все данные в структуру
  DateTime now = rtc.getTime();

  // меняем любой параметр
  now.year += 5;
  // now.second;
  // now.minute;
  // now.hour;
  // now.day;
  // now.date;
  // now.month;

  // отправляем в rtc
  rtc.setTime(now);
}

void loop() {
  printTime();
  delay(500);
}

void printTime() {
  // получаем все данные в структуру и используем их
  // этот способ быстрее и "легче" вызова отдельных get-функций
  DateTime now = rtc.getTime();

  Serial.print(now.hour);
  Serial.print(":");
  Serial.print(now.minute);
  Serial.print(":");
  Serial.print(now.second);
  Serial.print(" ");
  Serial.print(now.day);
  Serial.print(" ");
  Serial.print(now.date);
  Serial.print("/");
  Serial.print(now.month);
  Serial.print("/");
  Serial.println(now.year);
}
