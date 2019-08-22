#include <GyverWDT.h>
/* пример работы ватчдога в комбинированном режиме */

void setup() {
  Serial.begin(9600);
  Serial.println("hello!"); // сигнализирует о старте программы
  watchdog_enable(INTERRUPT_RESET_MODE, WDT_PRESCALER_512, alert); // подробнее о режимах и прескалерах в файле GyverWDT.h
  Serial.println("watchdog enabled"); // сигнализирует о включении watchdog
  while (1); // источник зависания
  Serial.println("data:"); // это сообщение мы никогда не увидим , из-за зависания выше
  /* первый таймаут сигнализирует предупреждением , последующий - инициирует сброс */
}

void alert() {
  Serial.println("attention!");
  // в этом прерывании вы можете попытаться исправить причину зависания
  // если нет - следующий таймаут инициирует сброс
}

void loop() {
}
