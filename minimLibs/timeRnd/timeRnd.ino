#include "TimeRandom.h"
#include "FastRandom.h"
TimeRandom rnd;
FastRandom frnd;

void setup() {
  Serial.begin(9600);
}

byte hour = 0;
byte minute = 0;
byte second = 0;

void loop() {
  if (++second >= 60) {
    second = 0;
    if (++minute >= 60) {
      minute = 0;
      if (++hour >= 24) {
        hour = 0;
      }
    }
  }
  rnd.update(hour, minute, second);
  Serial.println(rnd.fromSec(2, 0, 10));
  delay(10);
}
