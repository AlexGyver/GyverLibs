/*
   Тест мини класса энкодера v2
*/

// пины энкодера
#define SW 0
#define DT 2
#define CLK 3

#include "encMinim.h"
// пин clk, пин dt, пин sw, направление (0/1)
encMinim enc(CLK, DT, SW, 1);

void setup() {
  Serial.begin(9600);
}

void loop() {
  enc.tick();
  if (enc.isTurn()) Serial.println("turn");
  if (enc.isLeft()) Serial.println("left");
  if (enc.isRight()) Serial.println("right");
  if (enc.isLeftH()) Serial.println("leftH");
  if (enc.isRightH()) Serial.println("rightH");
  if (enc.isClick()) Serial.println("click");
  if (enc.isHolded()) Serial.println("hold");  
}
