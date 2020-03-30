#include "PWMrelay.h"
PWMrelay relay(13); // реле на 13 пине

// или так
// PWMrelay relay(13, HIGH); // реле высокого уровня на 13 пине
// PWMrelay relay(13, HIGH, 2000); // реле высокого уровня на 13 пине, период 2 секунды

void setup() {
  Serial.begin(9600);
  
  relay.setLevel(HIGH);   // можно поменять уровень реле (HIGH/LOW)
  
  relay.setPeriod(1000);  // можно поменять период, миллисекунды

  relay.setPWM(20);       // задаём сигнал ШИМ 0-255
}

void loop() {
  // вызываем в лупе, данная функция сама управляет реле
  relay.tick();   
}