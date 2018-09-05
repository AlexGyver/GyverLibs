/*
   Пример альфа-бета фильтра
*/

#include "GyverFilters.h"

// параметры: период дискретизации (измерений), process variation, noise variation
GABfilter testFilter(0.08, 40, 1);

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
