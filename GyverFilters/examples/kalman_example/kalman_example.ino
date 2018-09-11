/*
   Пример простого одномерного фильтра
*/

#include "GyverFilters.h"

// параметры: разброс измерения, разброс оценки, скорость изменения значений
// разброс измерения: шум измерений
// разброс оценки: подстраивается сам, можно поставить таким же как разброс измерения
// скорость изменения значений: 0.001-1, варьировать самому

GKalman testFilter(40, 40, 0.5);

// также может быть объявлен как (разброс измерения, скорость изменения значений)
// GKalman testFilter(40, 0.5);

void setup() {
  Serial.begin(9600);
}

void loop() {
  delay(80);
  int value = analogRead(0);
  value += random(2) * random(-1, 2) * random(10, 70);
  Serial.print("$");
  Serial.print(value);
  Serial.print(" ");
  value = testFilter.filtered((int)value);
  Serial.print(value);
  Serial.println(";");
}
