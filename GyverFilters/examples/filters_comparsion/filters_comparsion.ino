/*
   Сравнение калмана и бегущего среднего
*/
#include "GyverFilters.h"

// параметры: разброс измерения, разброс оценки, скорость изменения значений
// разброс измерения: шум измерений
// разброс оценки: подстраивается сам, можно поставить таким же как разброс измерения
// скорость изменения значений: 0.001-1, варьировать самому

GKalman kalman(90, 90, 0.5);
GFilterRA average(0.5, 80);

void setup() {
  Serial.begin(9600);
}

void loop() {  
  int value = analogRead(0);
  value += random(2) * random(-1, 2) * random(50, 100);
  Serial.print("$");
  Serial.print(value);
  Serial.print(" ");  
  
  Serial.print((int)kalman.filtered(value));
  Serial.print(" ");
  Serial.print((int)average.filtered(value));
  Serial.println(";");
  delay(80);
}
