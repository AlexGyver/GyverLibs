#include "GyverHacks.h"
GTimer myTimer;               // создать таймер

void setup() {
  Serial.begin(9600);
  myTimer.setInterval(2000);   // настроить интервал 2 сек
  myTimer.setMode(MANUAL);     // ручной режим
  Serial.println("Start");
}

void loop() {
  if (myTimer.isReady()) Serial.println("Timer!");
}
