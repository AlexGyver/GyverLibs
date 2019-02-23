// при изменении ШИМ у таймера 0 функции времени начинают работать 
// некорректно: быстрее или медленнее
// проблему решаем при помощи fixed функций, где учтён предделитель таймера
// delayFix()
// delayMicrosecondsFix()
// millisFix()
// microsFix()

#include "GyverHacks.h"

void setup() {
  Serial.begin(9600);
  setPWMprescaler(5, 2);  // пин 5 частота 7,8 кГц
}

void loop() {
  Serial.println("millis: " + String(millis()) + ", millisFix: " + String(millisFix()));
  delayFix(1000);
}
