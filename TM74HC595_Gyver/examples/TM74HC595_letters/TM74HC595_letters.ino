/*
   Пример работы с дисплеем на микросхеме TM74HC595
   Динамическая индикация осуществляется по прерываниям таймера Timer1
   Скетч показывает, как использовать заданные в библиотеке готовые буквы
   AlexGyver Technologies http://alexgyver.ru/
*/

//---- ПИНЫ ----
byte SCLK = 4;
byte RCLK = 3;
byte DIO = 2;
//---- ПИНЫ ----

#include <TM74HC595Display.h>
#include <TimerOne.h>
TM74HC595Display disp(SCLK, RCLK, DIO);

void setup() {
  Timer1.initialize(1500);           // установка таймера на каждые 1500 микросекунд
  Timer1.attachInterrupt(timerIsr);  // запуск таймера

  // вывод HELL
  disp.set(_H, 3);    // вывести H в четвёртый порт
  delay(1000);
  disp.set(_E, 2);     // вывести E в третий порт
  delay(1000);
  disp.set(_L, 1);    // вывести L во второй порт
  delay(1000);
  disp.set(_L, 0);    // вывести L в первый порт
  delay(2000);
}

void loop() {
  
}

void timerIsr() {   // прерывание таймера
  disp.timerIsr();  // пнуть дисплей
}
