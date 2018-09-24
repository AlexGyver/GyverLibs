// пример с таймером на micros

#include "GyverTimer.h"
GTimer_us myTimer(1500);    // создать таймер 1500 мкс = 1,5 миллисекунды

void setup() {
  Serial.begin(9600);
}

void loop() {
  if (myTimer.isReady()) Serial.println("Timer!");
}