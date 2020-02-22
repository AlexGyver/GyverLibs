// Пример генерации двухтактного меандра на таймере 2 (пины D3 и D11)
#include <GyverTimers.h>

void setup() {
  pinMode(3, OUTPUT);                           // настроить пин как выход  
  pinMode(11, OUTPUT);                          // настроить пин как выход
  Timer2.setFrequency(30000);                   // настроить частоту таймера в Гц и запустить таймер
  Timer2.outputEnable(CHANNEL_A, TOGGLE_PIN);   // в момент срабатывания таймера пин будет переключаться
  Timer2.outputEnable(CHANNEL_B, TOGGLE_PIN);   // в момент срабатывания таймера пин будет переключаться
  Timer2.outputState(CHANNEL_A, HIGH);          // задаём начальное состояние пина 11
  Timer2.outputState(CHANNEL_B, LOW);           // задаём начальное состояние пина 3  
}

void loop() {
}
