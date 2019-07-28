#include <directADC.h>
/* пример использования компаратора*/
void setup() {
  Serial.begin(9600);
}
// компаратор уже работает на ногах : +in => (ADC_AIN0 - pin 6)   -in => (ADC_AIN1 - pin 7);
void loop() {
  Serial.println(ACOMP_read()); // прочитать выход компаратора
}
