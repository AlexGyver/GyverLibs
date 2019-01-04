/*
 * Отработка по прерыванию таймера
 */

#define CLK 7
#define DT 8
#define SW 9

#include "GyverEncoder.h"
#include "TimerOne.h"
Encoder enc1(CLK, DT, SW);

void setup() {
  Serial.begin(9600);
  enc1.setType(TYPE2);    // тип энкодера TYPE1 одношаговый, TYPE2 двухшаговый. Если ваш энкодер работает странно, смените тип

  Timer1.initialize(1000);           // установка таймера на каждые 1000 микросекунд (= 1 мс)
  Timer1.attachInterrupt(timerIsr);   // запуск таймера
}

void timerIsr() {   // прерывание таймера
  enc1.tick();     // отработка теперь находится здесь
}

void loop() {
  if (enc1.isRight()) Serial.println("Right");         // если был поворот
  if (enc1.isLeft()) Serial.println("Left");
  if (enc1.isRightH()) Serial.println("Right holded"); // если было удержание + поворот
  if (enc1.isLeftH()) Serial.println("Left holded");
  
}
