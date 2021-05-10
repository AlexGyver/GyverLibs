#include "GyverOS.h"
GyverOS<5> OS;
void setup() {
  Serial.begin(9600);
  pinMode(13, OUTPUT);
  OS.attach(0, f1, 400);
  OS.attach(1, f2, 1000);
  OS.attach(2, f3, 200);
  //OS.enableLoad(3000);
}

void loop() {
  OS.tick();
  delay(OS.getLeft());
}

void f1() {
  static uint32_t ms;
  Serial.println(millis() - ms);
  ms = millis();
}

void f2() {
  static uint32_t ms;
  Serial.println(millis() - ms);
  ms = millis();
}

void f3() {
  digitalWrite(13, !digitalRead(13));
  static uint32_t ms;
  Serial.println(millis() - ms);
  ms = millis();
}
