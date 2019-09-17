// работа с "минимальным" инструментом для ds18b20

#define DS_PIN 2  // пин датчика
#include "ds18b20minim.h"

void setup() {
  Serial.begin(9600);
  dallas_begin(DS_PIN); // инициализация  
}
void loop() {
  dallas_requestTemp(DS_PIN); // запрос
  delay(1000);	// ~1 секунду занимает преобразование. Нет смысла опрашивать чаще!
  Serial.println(dallas_getTemp(DS_PIN)); // получаем температуру
}
