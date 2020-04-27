#include "thermistorMinim.h"
thermistor therm(0, 10000, 3435);	// пин, сопротивление при 25 градусах (R термистора = R резистора!), бета-коэффициент
// thermistor therm(0, 10000, 3435, 25, 10000);	// пин, R термистора, B термистора, базовая температура, R резистора
	

void setup() {
  Serial.begin(9600);
}

void loop() {
  Serial.print("Temperature ");
  Serial.print(therm.getTempAverage());
  Serial.println(" *C");
  delay(500);
}
