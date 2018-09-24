#include "GyverTimer.h"
GTimer_ms myTimer;               // создать таймер

void setup() {
  Serial.begin(9600);
  myTimer.setInterval(500);   // настроить интервал
}

void loop() {
  if (myTimer.isReady()) Serial.println("Timer!");
}
