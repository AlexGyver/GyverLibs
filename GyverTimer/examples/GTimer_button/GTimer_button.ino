// пример с кнопкой, кнопка сбрасывает таймер

#include <GyverButton.h>  // библиотека кнопки
GButton btn(3);           // кнопка на D3

#include "GyverTimer.h"   // библиотека таймера
GTimer myTimer(MS);       // создать таймер (по умолч. в режиме интервала)

void setup() {
  Serial.begin(9600);
  myTimer.setTimeout(1000); // запуск в режиме таймаута 1 секунда
}

void loop() {  
  btn.tick();   // опрос кнопки
  // при нажатии кнопки задаём таймаут 1 секунду   
  if (btn.isClick()) myTimer.setTimeout(1000);

  // через секунду после последнего нажатия выведет Timeout!
  if (myTimer.isReady()) Serial.println("Timeout!");
}
