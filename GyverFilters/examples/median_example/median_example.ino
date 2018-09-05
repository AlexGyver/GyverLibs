/*
	Пример использования медианного фильтра.
	Порядок фильтра настраивается в GyverHacks.h - MEDIAN_FILTER_SIZE
*/

#include "GyverFilters.h"
GMedian testFilter;

void setup() {
  Serial.begin(9600);
}

void loop() {
  delay(80);
  int value = analogRead(0);
  // добавляем шум "выбросы"
  value += random(2) * random(2) * random(-1, 2) * random(50, 250);
  Serial.print("$"); 
  Serial.print(value);
  Serial.print(" ");  
  value = testFilter.filtered(value);
  Serial.print(value);
  Serial.println(";"); 
}
