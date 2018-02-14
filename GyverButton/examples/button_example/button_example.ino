#define PIN 3				// кнопка подключена сюда (PIN --- КНОПКА --- GND)

#include "GyverButton.h"
GButton butt1(PIN);

void setup() {
  Serial.begin(9600);
  
  butt1.setDebounce(50);    // настройка антидребезга (по умолчанию 80)
  butt1.setTimeout(300);    // настройка таймаута (по умолчанию 500)
}

void loop() {
  butt1.tick();  // обязательная функция отработки. Должна постоянно опрашиваться
  
  if (butt1.isDouble()) Serial.println("Double");       // проверка на двойной клик
  if (butt1.isTriple()) Serial.println("Triple");       // проверка на тройной клик

  if (butt1.hasClicks())                                // проверка на наличие нажатий
    Serial.println(butt1.getClicks());                  // получить (и вывести) число нажатий

  if (butt1.isPress()) Serial.println("Press");         // нажатие на кнопку (+ дебаунс)
  if (butt1.isRelease()) Serial.println("Release");     // отпускание кнопки (+ дебаунс)
  if (butt1.isHolded()) Serial.println("Holded");       // проверка на удержание
  //if (butt1.isHold()) Serial.println("Hold");         // возвращает состояние кнопки

}
