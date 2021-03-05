#include "Timer.h"

Timer tmr(2000, 1);

void setup() {
  Serial.begin(9600);
  pinMode(A0, INPUT_PULLUP);
}

void loop() {
  if (tmr.ready()) Serial.println("period");
  //Serial.println(tmr.timeLeft8());
  delay(10);
  if (click()) {
    static bool flag = 1;
    if (flag) tmr.stop();
    else tmr.resume();
    flag = !flag;
    //tmr.start();
  }
}

bool click() {
  static uint32_t tmr = 0;
  if (!digitalRead(A0) && millis() - tmr > 100) {
    tmr = millis();
    return true;
  }
  return false;
}
