// пример с кнопкой, которая приостанавливает таймер

#include <GyverButton.h>  // библиотека кнопки
GButton btn(3);           // кнопка на D3

#include "GyverTimer.h"   // библиотека таймера
GTimer myTimer(MS);       // создать таймер (по умолч. в режиме интервала)

void setup() {
  Serial.begin(9600);
  myTimer.setInterval(5000); // запуск в режиме таймаута 5 секунд
}

void loop() {
  btn.tick();   // опрос кнопки
  
  // при нажатии кнопки останавливаем и продолжаем таймер
  if (btn.isClick()) {
    static bool flag = false;
    flag = !flag;
    if (flag) myTimer.stop();
    else myTimer.resume();
  }

  // оповещение о таймауте
  if (myTimer.isReady()) Serial.println("Timeout!");
}
