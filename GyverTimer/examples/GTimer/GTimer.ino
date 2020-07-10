#include "GyverTimer.h"
GTimer myTimer(MS);               // создать миллисекундный таймер

void setup() {
  Serial.begin(9600);
  myTimer.setInterval(500);   // настроить интервал
}

void loop() {
  if (myTimer.isReady()) Serial.println("Timer!");
}
