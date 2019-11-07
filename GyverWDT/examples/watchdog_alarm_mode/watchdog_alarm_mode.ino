#include <GyverWDT.h>
#include <avr/sleep.h>

/*
   Пример использования watchdog в "будильника"
   Зависимость таймаутов от делителей см. в GyverWDT.h
*/

void setup() {
  pinMode(13, OUTPUT);
  set_sleep_mode(SLEEP_MODE_PWR_DOWN); // Выбираем нужный режим сна
}

void loop() {

  /* Мигаем светодиодом , а в паузах спим */
  digitalWrite(13, HIGH);

  Watchdog.enable(INTERRUPT_MODE, WDT_PRESCALER_128); // Режим прерываний , таймаут ~1c
  sleep_enable();                                     // Разрешаем сон
  sleep_cpu();                                        // Уходим в сон

  digitalWrite(13, LOW);

  Watchdog.enable(INTERRUPT_MODE, WDT_PRESCALER_128); // Режим прерываний , таймаут ~1c
  sleep_enable();                                     // Разрешаем сон
  sleep_cpu();                                        // Уходим в сон

}

/* Прерывание watchdog , в нем мы просыпаемся */
ISR(WATCHDOG) {
  sleep_disable();     // Запрещаем сон
  Watchdog.disable();  // Выключаем watchdog
}
