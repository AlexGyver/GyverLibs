#include <GyverTimer1.h>

void setup() {
  Serial.begin(9600);
  timer1_setPeriod(500000);   // ставим период на 500 000 мкс (0.5 с)

  // то же самое, но в Герцах
  // timer1_setFrequency(2);    // ставим 2 герца

  timer1_ISR(handler);    // подключить прерывание
  timer1_start();         // запустить таймер

  // timer1_stop();       // остановить таймер
  // timer1_resume();     // продолжить таймер
  // timer1_restart();    // перезапустить таймер
}

void handler() {
  Serial.println("isr!");
}

void loop() {

}
