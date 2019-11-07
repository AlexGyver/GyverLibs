#include <GyverWDT.h>

/*
  Пример классического применения watchdog - сброс устройства при зависании
  Если не вызвать Watchdog.reset() вовремя - произойдет сброс
  Зависимость таймаутов от делителей см. в GyverWDT.h
*/

void setup() {
  pinMode(13, OUTPUT);
  Watchdog.enable(RESET_MODE, WDT_PRESCALER_512); // Режим сторжевого сброса , таймаут ~4с
}

void loop() {
  /* Ваш полезный код */
  digitalWrite(13, HIGH);
  delay(1000);
  digitalWrite(13, LOW);
  delay(1000);
   
  Watchdog.reset(); // Переодический сброс watchdog, означающий, что устройство не зависло
}
