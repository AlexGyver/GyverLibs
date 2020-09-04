#include "noise.h"

void setup() {
  Serial.begin(9600);
}

void loop() {
  static int val = 0;
  val += 20;
  Serial.println(inoise8(val, 3214));
  delay(10);
}
