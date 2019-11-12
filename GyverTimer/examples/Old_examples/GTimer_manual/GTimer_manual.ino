/*
	Пример работы в ручном режиме (отложенный пуск)
*/

#include "GyverTimer.h"
GTimer_ms myTimer;                // создать таймер

void setup() {
  Serial.begin(9600);
  myTimer.setInterval(5000);   // настроить интервал 5 сек
  myTimer.setMode(MANUAL);     // ручной режим
  Serial.println("Start");
  Serial.println("Wait 5 sec");
}

void loop() {
  if (myTimer.isReady()) Serial.println("Timer!");
}
