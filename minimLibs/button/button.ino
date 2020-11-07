/*
   Тест мини класса кнопки
*/
#define SW 7    // пин кнопки, подключена на него и GND

#include "buttonMinim.h"
buttonMinim butt(SW);

void setup() {
  Serial.begin(9600);
}

void loop() {
  if (butt.pressed()) Serial.println("press");
  if (butt.clicked()) Serial.println("clicked");
  if (butt.holded()) Serial.println("holded");
  //if (butt.holding()) Serial.println("holding");
}
