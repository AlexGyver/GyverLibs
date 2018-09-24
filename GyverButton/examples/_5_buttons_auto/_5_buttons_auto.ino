/*
   Пример использования библиотеки GyverButton, 5 кнопок
   Режим опроса - автоматический
*/

#define BTN1 3			// кнопка подключена сюда (PIN --- КНОПКА --- GND)
#define BTN2 4        	// кнопка подключена сюда (PIN --- КНОПКА --- GND)
#define BTN3 5        	// кнопка подключена сюда (PIN --- КНОПКА --- GND)
#define BTN4 6        	// кнопка подключена сюда (PIN --- КНОПКА --- GND)
#define BTN5 7        	// кнопка подключена сюда (PIN --- КНОПКА --- GND)

#include "GyverButton.h"
GButton butt1(BTN1);
GButton butt2(BTN2);
GButton butt3(BTN3);
GButton butt4(BTN4);
GButton butt5(BTN5);

void setup() {
  Serial.begin(9600);
  butt1.setTickMode(AUTO);
  butt2.setTickMode(AUTO);
  butt3.setTickMode(AUTO);
  butt4.setTickMode(AUTO);
  butt5.setTickMode(AUTO);
}

void loop() {
  if (butt1.isClick()) Serial.println("Button 1");
  if (butt2.isClick()) Serial.println("Button 2");
  if (butt3.isClick()) Serial.println("Button 3");
  if (butt4.isClick()) Serial.println("Button 4");
  if (butt5.isClick()) Serial.println("Button 5");
}
