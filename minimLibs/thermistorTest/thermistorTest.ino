#include "thermistorMinim.h"
thermistor therm(0, 10000, 3435);

void setup() {
  Serial.begin(9600);
}

void loop() {
  Serial.print("Temperature ");
  Serial.print(therm.getTempAverage());
  Serial.println(" *C");
  delay(500);
}
