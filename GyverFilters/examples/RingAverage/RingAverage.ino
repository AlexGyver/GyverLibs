#include "GyverFilters.h"
RingAverage<int, 50> fil;

void setup() {
  Serial.begin(9600);  
}

void loop() {
  Serial.println(fil.filtered(random(50)));
  delay(10);
}
