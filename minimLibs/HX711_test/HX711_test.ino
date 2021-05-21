#include "HX711.h"
HX711 sensor(7, 8, HX_GAIN128_A);

void setup() {
  Serial.begin(9600);
  sensor.tare();
}

void loop() {
  if (sensor.available()) {
    Serial.println(sensor.read());
  }
}
