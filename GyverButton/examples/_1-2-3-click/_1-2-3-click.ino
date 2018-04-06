/*
   Пример использования библиотеки GyverButton, 1- 2- 3- нажатие
*/

#define PIN 3				// кнопка подключена сюда (PIN --- КНОПКА --- GND)

#include "GyverButton.h"
GButton butt1(PIN);

void setup() {
  Serial.begin(9600);
}

void loop() {
  butt1.tick();  // обязательная функция отработки. Должна постоянно опрашиваться

  if (butt1.isSingle()) Serial.println("Single");       // проверка на один клик
  if (butt1.isDouble()) Serial.println("Double");       // проверка на двойной клик
  if (butt1.isTriple()) Serial.println("Triple");       // проверка на тройной клик
}
