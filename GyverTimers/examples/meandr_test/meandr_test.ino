//Пример генерации меандра на таймере 2 , канале B (D3 на Arduino UNO) 
#include <GyverTimers.h>

void setup() {
  pinMode(3, OUTPUT);                           // настроить пин как выход 
  Timer2.setFrequency(500);                     // настроить частоту таймера в Гц
  Timer2.outputEnable(CHANNEL_B, TOGGLE_PIN);   // в момент срабатывания таймера пин будет переключаться
}

void loop() {
}
