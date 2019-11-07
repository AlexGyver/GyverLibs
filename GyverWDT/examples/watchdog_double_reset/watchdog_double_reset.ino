#include <GyverWDT.h>

/*
   Пример работы watchdog для контроля сразу за двумя устройствами
   В данном случае - arduino + esp8266 модуль
   Зависимость таймаутов от делителей см. в GyverWDT.h
*/

void setup() {
  Serial.begin(9600);                                         // На этом последовательном порту у нас wifi-модуль
  pinMode(8, INPUT);                                          // Этот пин сбрасывает wifi-модуль при переходе в режим OUTPUT
  Watchdog.enable(INTERRUPT_RESET_MODE, WDT_PRESCALER_1024);  // Комбинированный сторжевой режим , таймаут ~8с
}


void loop() {
  /* Ваш полезный код */
}

/* Если wifi-модуль нам что то отправил , а мы его поняли - никто из нас не завис */
void serialEvent() {
  Watchdog.reset();  // Сбросить watchdog
}

/* Если wifi-модуль не отправлял нам что то более 8с - он завис */
ISR(WATCHDOG) {
  pinMode(8, OUTPUT);                                         // Тянем за reset модуля
  Watchdog.enable(INTERRUPT_RESET_MODE, WDT_PRESCALER_1024);  // Если мы не смогли перенастроить watchdog - значит мы зависли
  Watchdog.reset();                                           // Если не зависли - сбросить watchdog и продолжить
  pinMode(8, INPUT);                                          // Отпускаем reset модуля
}
