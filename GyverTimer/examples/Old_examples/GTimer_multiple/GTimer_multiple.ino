#include "GyverTimer.h"

// создать таймер, в скобках период в миллисекундах
GTimer_ms myTimer1(500);
GTimer_ms myTimer2(600);
GTimer_ms myTimer3(1000);

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
