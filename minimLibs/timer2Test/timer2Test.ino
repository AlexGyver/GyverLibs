/*
   Тест мини класса таймера v2.0
*/

#include "timer2Minim.h"
timerMinim timer1(1000);    // 1000мс таймер 
timerMinim timer3(3000);    // 3000мс таймер

void setup() {
  Serial.begin(9600);
}

void loop() {
  if (timer1.isReady()) Serial.println("timer 1!");
  if (timer3.isReady()) Serial.println("timer 3!");
}
