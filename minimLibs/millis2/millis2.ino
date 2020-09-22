#include "uptime2.h"

void setup() {
  Serial.begin(9600);
  uptime2Init();
}

void loop() {
  Serial.println(millis2());
  delay2(1000);
}
