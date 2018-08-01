/*
   Пример использования библиотеки GyverButton, отработка любого количества нажатий
*/

#define PIN 3				// кнопка подключена сюда (PIN --- КНОПКА --- GND)

#include "GyverButton.h"
GButton butt1(PIN);

void setup() {
  Serial.begin(9600);
  butt1.setTimeout(300);        // настройка таймаута на удержание и второй клик (по умолчанию 500 мс)
}

void loop() {
  butt1.tick();  // обязательная функция отработки. Должна постоянно опрашиваться

  if (butt1.hasClicks())                     // проверка на наличие нажатий
    Serial.println(butt1.getClicks());       // получить (и вывести) количество нажатий
}
