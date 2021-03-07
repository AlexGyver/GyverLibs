#include "UnixDate.h"

void setup() {
  Serial.begin(9600);
  UnixDate date(1615153965, 3);	// unix time, GMT
  Serial.println(date.year);
  Serial.println(date.month);
  Serial.println(date.day);
  Serial.println(date.dayOfWeek);
  Serial.println(date.hours);
  Serial.println(date.minutes);
  Serial.println(date.seconds);
}

void loop() {

}
