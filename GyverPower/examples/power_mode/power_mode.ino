// пример настроек сна
#include <GyverPower.h>

void setup() {
  pinMode(13, OUTPUT);    // шоб мигать
  
  // отключение ненужной периферии
  power.hardwareDisable(PWR_ADC | PWR_TIMER1); // см. константы, разделяющий знак " | "
  // можно отключить всё
  // hardwareDisable(PWR_ALL);

  power.setSleepMode(STANDBY_SLEEP); // режим сна (по умолчанию POWERDOWN_SLEEP)
  //bodInSleep(false); // отключение BOD (по у молчанию и так выключен) - экономит энергию
}

void loop() {
  power.sleep(SLEEP_2048MS); // спим ~ 2 секунды (некалиброванный таймаут. Смотри пример с калибрвокой!)
  digitalWrite(13, !digitalRead(13)); // инвертируем состояние на пине
}
