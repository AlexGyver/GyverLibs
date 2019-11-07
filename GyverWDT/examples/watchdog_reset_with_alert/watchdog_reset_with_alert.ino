#include <GyverWDT.h>

/*
   Пример использования watchdog в комбинированном режиме
   Может использоваться для оповещения в случае зависания
   Первый тайм-аут вызывает прерывание, второе - сброс
   Перенастройте watchdog чтобы избежать сброса
   Зависимость таймаутов от делителей см. в GyverWDT.h
*/

void setup() {
  Serial.begin(9600);
  Serial.println("Program started");
  Watchdog.enable(INTERRUPT_RESET_MODE, WDT_PRESCALER_128);   // Комбинированный режим , таймаут ~1c
  Serial.println("watchdog enabled");
  while (1);                                                  // Причина зависания
  Serial.println("loop started");                             // Этого мы не увидим из-за зависания
}

void loop() {
  /* этого кода программа не достигнет - зависание в setup */
  Serial.println("hello");
  delay(500);
}

/* Первый тайм-аут вызывает прерывание */
ISR(WATCHDOG) {
  // Если причина зависания программная - тут можно попытаться исправить ее */
  Serial.println("warning!");
  // Если исправить причину не вышло - следующий таймаут вызывает сброс
  // Watchdog.enable(INTERRUPT_RESET_MODE, WDT_PRESCALER_128); // Если перенастроить watchdog здесь - сброса не будет
}
