// пример параллельной работы нескольких таймеров

#include "GyverTimer.h"   // подключаем библиотеку

// создаём таймеры в миллисекундах
GTimer myTimer1(MS, 500);
GTimer myTimer2(MS, 600);
GTimer myTimer3(MS, 1000);

void setup() {
  Serial.begin(9600);
}

void loop() {
  if (myTimer1.isReady())
    Serial.println("Timer 1!");

  if (myTimer2.isReady())
    Serial.println("Timer 2!");

  if (myTimer3.isReady())
    Serial.println("Timer 3!");
}
