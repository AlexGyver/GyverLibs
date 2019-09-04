// тест получения времени и даты компиляции

#include "buildTime.h"
void setup() {
  Serial.begin(9600);
  Serial.println(__DATE__);
  Serial.println(__TIME__);
  Serial.println();
  
  Serial.print(BUILD_YEAR);
  Serial.print(" ");
  Serial.print(BUILD_MONTH);
  Serial.print(" ");
  Serial.println(BUILD_DAY);

  Serial.print(BUILD_HOUR);
  Serial.print(":");
  Serial.print(BUILD_MIN);
  Serial.print(":");
  Serial.println(BUILD_SEC);
}

void loop() {

}
