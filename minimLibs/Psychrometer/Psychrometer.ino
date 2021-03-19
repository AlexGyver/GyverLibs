#include "Psychrometer.h"
Psychrometer psy;

void setup() {
  Serial.begin(9600);
  Serial.println(psy.getHumidity(15.0, 12.5));
}


void loop() {}
