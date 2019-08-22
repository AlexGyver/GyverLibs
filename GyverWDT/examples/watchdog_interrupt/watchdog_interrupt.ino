#include <GyverWDT.h>
/* пример работы ватчдога как источника переодических прерываний */

volatile long oldTime;
void setup() {
  Serial.begin(9600);
  Serial.println("hello!");
  watchdog_enable(INTERRUPT_MODE, WDT_PRESCALER_128, func); // подробнее о делителях и режимах в GyverWDT.h
  oldTime = millis();
  delay(10000);  // работаем 10 секунд
  watchdog_disable(); // выключаем ватчдог
  //while(1) watchdog_reset(); // или можем сбрасывать его и не допускать таймаута и соотв. прерываний
}

void func() {
  Serial.println(millis() - oldTime);
  oldTime = millis();
}

void loop() {}
