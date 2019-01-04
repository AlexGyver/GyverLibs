#include "GyverEncoder.h"
Encoder enc1(4, 3, 2);
Encoder enc2(7, 6, 5);

void setup() {
  Serial.begin(9600);
  
  // установка начальной точки для поворота
  enc1.setCounterNorm(0);
  enc2.setCounterNorm(50);

  // установка шага при повороте
  enc1.setStepNorm(1);
  enc2.setStepNorm(5);

  // устанвока пределов
  enc1.setLimitsNorm(0, 10);
  enc2.setLimitsNorm(0, 100);
}

void loop() {
	// обязательная функция отработки. Должна постоянно опрашиваться
  enc1.tick();
  enc2.tick();
  
  if (enc1.isTurn()) Serial.println(enc1.normCount);  // получить счётчик обычный
  if (enc2.isTurn()) Serial.println(enc2.normCount);  // получить счётчик обычный  
}
