#include "GyverHacks.h"

// создать таймер, в скобках период в миллисекундах
GTimer myTimer1(500);
GTimer myTimer2(600);
GTimer myTimer3(1000);

void setup() {
  Serial.begin(9600);
}

void loop() {
  if (myTimer1.isReady())
    Serial.println("Timer 1!");

  if (myTimer2.isReady())
    Serial.println("Timer 2!");

  if (myTimer3.isReady())
    Serial.println("Timer 3!");
}
