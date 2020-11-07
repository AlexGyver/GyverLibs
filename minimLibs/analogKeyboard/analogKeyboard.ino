/*
  Сигналы кнопок
  1023
  927
  856
  783
  671
  632
  590
  560
  504
  480
  455
  440
  399
  319
  255
  230
*/
#include "AnalogKey.h"
// указываем пин и количество кнопок
AnalogKey<A0, 16> keys;

void setup() {
  Serial.begin(9600);
  
  // назначаем кнопкам их сигналы
  keys.attach(0, 1023);
  keys.attach(1, 927);
  keys.attach(2, 856);
  keys.attach(3, 783);
  keys.attach(4, 671);
  keys.attach(5, 632);
  keys.attach(6, 590);
  keys.attach(7, 560);
  keys.attach(8, 504);
  keys.attach(9, 480);
  keys.attach(10, 455);
  keys.attach(11, 440);
  keys.attach(12, 399);
  keys.attach(13, 319);
  keys.attach(14, 255);
  keys.attach(15, 230);
}

void loop() {  
  // проверяем каждую кнопку в ручном режиме
  if (keys.status(0)) Serial.println("press 0");
  if (keys.status(1)) Serial.println("press 1");
  if (keys.status(2)) Serial.println("press 2");
  if (keys.status(3)) Serial.println("press 3");
  if (keys.status(4)) Serial.println("press 4");
  if (keys.status(5)) Serial.println("press 5");
  if (keys.status(6)) Serial.println("press 6");

  // или выводим номер текущей нажатой (-1 значит ни одна не нажата)
  if (keys.pressed() != -1) Serial.println(keys.pressed());
  delay(10);
}
