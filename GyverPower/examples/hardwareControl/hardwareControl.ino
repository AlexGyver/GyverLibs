// управление периферией МК
#include <GyverPower.h>

void setup() {
  Serial.begin(9600);
  // можно выборочно включать-выключать периферийные блоки МК, разделитель - |
  // список констант смотри в GyverPower.h / Константы периферии
  
  // отключили АЦП, таймеры 1 и 0
  power.hardwareDisable(PWR_ADC | PWR_TIMER1 | PWR_TIMER0);
  
  // включили ADC обратно
  power.hardwareEnable(PWR_ADC);
}

void loop() {
  Serial.println(millis());
  // отключили таймер 0 - миллис не тикает...
}
