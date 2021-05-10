// демо возможностей библиотеки
#include <microDS3231.h>
MicroDS3231 rtc;

void setup() {
  Serial.begin(9600);
  
  if (rtc.lostPower()) {  //  при потере питания
    rtc.setTime(COMPILE_TIME);  // установить время компиляции
  }

  //rtc.setTime(SEC, MIN, HOUR, DAY, MONTH, YEAR); // устанвока времени вручную
}

void loop() {
  printTime();
  delay(500);
}

void printTime() {
  Serial.print(rtc.getHours());
  Serial.print(":");
  Serial.print(rtc.getMinutes());
  Serial.print(":");
  Serial.print(rtc.getSeconds());
  Serial.print(" ");
  Serial.print(rtc.getDay());
  Serial.print(" ");
  Serial.print(rtc.getDate());
  Serial.print("/");
  Serial.print(rtc.getMonth());
  Serial.print("/");
  Serial.println(rtc.getYear());
  
  Serial.println(rtc.getTemperatureFloat());  
  //Serial.println(rtc.getTemperature());
  Serial.println(rtc.getTimeString());
  Serial.println(rtc.getDateString());

  // работа с char
  char time[8];
  rtc.getTimeChar(time);  // записать в массив time
  Serial.println(time);
  
  char date[10];
  rtc.getDateChar(date);
  Serial.println(date);
  
  Serial.println();
}
