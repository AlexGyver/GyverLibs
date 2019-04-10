/*
   Пример работы с энкодером с прерыванием. Максимальная чёткость работы
   в любом быдлокоде!
*/

#define CLK 2
#define DT 3
#define SW 4

#include "GyverEncoder.h"
Encoder enc1(CLK, DT, SW);

void setup() {
  Serial.begin(9600);
  attachInterrupt(0, isrCLK, CHANGE);    // прерывание на 2 пине! CLK у энка
  attachInterrupt(1, isrDT, CHANGE);    // прерывание на 3 пине! DT у энка
}

void isrCLK() {
  enc1.tick();  // отработка в прерывании
}
void isrDT() {
  enc1.tick();  // отработка в прерывании
}

void loop() {
  enc1.tick();
  if (enc1.isRight()) Serial.println("Right");         // если был поворот
  if (enc1.isLeft()) Serial.println("Left");

  if (enc1.isRightH()) Serial.println("Right holded"); // если было удержание + поворот
  if (enc1.isLeftH()) Serial.println("Left holded");
}
