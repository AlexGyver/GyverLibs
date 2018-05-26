#include "GyverHacks.h"

void setup() {
  Serial.begin(9600);
  Serial.println(getVCC());         // напряжение питания в милливольтах
  Serial.println(getVoltage(0));    // напряжение на аналоговом пине в мВ
  Serial.println(getTemp());        // температура ядра

}

void loop() {
  
}


