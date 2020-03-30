#include "PWMrelay.h"
PWMrelay heater1(4);
PWMrelay heater2(5);

void setup() {
  heater1.setPWM(20);       // задаём сигнал ШИМ 0-255
  heater2.setPWM(120);       // задаём сигнал ШИМ 0-255
}

void loop() {
  heater1.tick();
  heater2.tick();
}
