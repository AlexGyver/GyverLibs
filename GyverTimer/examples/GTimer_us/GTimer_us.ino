// пример с таймером на micros

#include "GyverTimer.h"
GTimer myTimer(US, 1500);    // создать таймер 1500 мкс = 1,5 миллисекунды

void setup() {
  Serial.begin(9600);
}

void loop() {
  if (myTimer.isReady()) Serial.println("Timer!");
}