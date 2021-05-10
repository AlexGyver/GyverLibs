#include "TimerMillis.h"
// (период, мс), (0 не запущен / 1 запущен), (режим: 0 период / 1 таймер)
TimerMillis tmr(2000, 1, 1);

void setup() {
  Serial.begin(9600);
  //tmr.setTimerMode();
  tmr.setPeriodMode();
}

void loop() {
  if (tmr.tick()) Serial.println("finish");
}
