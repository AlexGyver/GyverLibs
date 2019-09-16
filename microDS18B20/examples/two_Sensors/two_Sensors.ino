#include <microDS18B20.h>

MicroDS18B20 sensor1(2);
MicroDS18B20 sensor2(3);

void setup() {
  Serial.begin(9600);  
}

void loop() {
  Serial.print("t: ");
  sensor1.requestTemp();
  sensor2.requestTemp();
  delay(1000);  
  Serial.print(sensor1.getTemp());
  Serial.print(" ");
  Serial.println(sensor2.getTemp());
}
