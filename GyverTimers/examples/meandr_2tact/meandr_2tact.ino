// Пример генерации двухтактного меандра на таймере 2 (пины D3 и D11)
#include <GyverTimers.h>

void setup() {
  pinMode(3, OUTPUT);                           // настроить пин как выход
  pinMode(11, OUTPUT);                          // настроить пин как выход

  // из-за особенности генерации меандра таймером
  // частоту нужно указывать в два раза больше нужной!
  Timer2.setFrequency(15000 * 2);               // настроить частоту в Гц и запустить таймер. Меандр на 15 кГц
  Timer2.outputEnable(CHANNEL_A, TOGGLE_PIN);   // в момент срабатывания таймера пин будет переключаться
  Timer2.outputEnable(CHANNEL_B, TOGGLE_PIN);   // в момент срабатывания таймера пин будет переключаться
  Timer2.outputState(CHANNEL_A, HIGH);          // задаём начальное состояние пина 11
  Timer2.outputState(CHANNEL_B, LOW);           // задаём начальное состояние пина 3
}

void loop() {
}