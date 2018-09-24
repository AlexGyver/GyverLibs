/*
  digitalWrite() - 3.2 мкс
  setPin() - 0.32 мкс (в 10 раз быстрее)

  digitalRead() - 3.08 мкс
  readPin() - 0.28 мкс (в 11 раз быстрее)

  analogWrite() - 3.87 мкс
  setPWM() - 0.53 мкс (в 7 раз быстрее)

  analogRead() стандартный - 113 мкс
  setADCrate(1) - установка максимальной скорости АЦП (по умолч. 7 - минимум)
  analogRead() на макс. скорости - 3.04 мкс (в 38 раз быстрее)
*/

#include "GyverHacks.h"
void setup() {
  Serial.begin(9600);
  setADCrate(1);
  /*
    // мигаем всеми пинами
    for (byte i = 0; i < 20; i++) {
    pinMode(i, OUTPUT);
    }
    for (byte i = 0; i < 20; i++) {
    setPin(i, 1);
    delay(400);
    setPin(i, 0);
    delay(200);
    }
  */
}

void loop() {

}
