/*
  Симуляция работы реле. Обнули k и увидишь, как регулятор
  перестанет справляться с инерционной системой
*/

#include "GyverRelay.h"
GyverRelay regulator(REVERSE);

void setup() {
  Serial.begin(9600);
  regulator.setpoint = 40;
  regulator.hysteresis = 5;
  regulator.k = 0.5;
}

boolean state = 0;
float value = 0;

void loop() {
  process();
  regulator.input = value;
  state = regulator.getResult();

  Serial.print(value);
  Serial.print(' ');
  Serial.print(regulator.setpoint - 5 / 2);
  Serial.print(' ');
  Serial.print(regulator.setpoint + 5 / 2);
  Serial.print(' ');
  Serial.println(regulator.setpoint);
  delay(100);
}

void process() {
  static float coef = 0;
  coef += state ? 0.3 : -0.6;
  if (coef > 2.0) coef = 2.0;
  if (coef < -3.0) coef = -3;
  value += coef;
}
