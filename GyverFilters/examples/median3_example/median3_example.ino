/*
	Пример использования быстрого медианного фильтра 3 порядка
*/

#include "GyverFilters.h"
GMedian3 testFilter;

void setup() {
  Serial.begin(9600);
}

void loop() {    
  int value = analogRead(0);
  // добавляем шум "выбросы"
  value += random(2) * random(2) * random(-1, 2) * random(50, 250);
  Serial.print("$"); 
  Serial.print(value);
  Serial.print(" ");  
  value = testFilter.filtered(value);
  Serial.print(value);
  Serial.println(";"); 
  delay(80);
}
