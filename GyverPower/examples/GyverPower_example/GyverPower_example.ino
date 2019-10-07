//#define FIX_TIME // Коррекция времени в случае изменения системной частоты
#include <GyverPower.h>

void setup() {
  pinMode(13, OUTPUT); // настраиваем вывод со светодиодом на выход
  Serial.begin(9600);

  // калибровка таймаутов для максимальной точности sleepDelay
  //Serial.println(getMaxTimeout()); // вывести реальный макс. период
  //calibrate(8935); 			// ввести реальный макс. период
  //calibrate(getMaxTimeout()); // автоматически посчитать и откалибровать
  autoCalibrate(); // автоматическая калибровка ~ 2 секунды , средняя но достаточная точность

  /* отключение ненужной периферии */
  hardwareDisable(PWR_ADC | PWR_TIMER1); // см константы, разделяющий знак " | "

  /* управление системной частотой */
  setSystemPrescaler(PRESCALER_2); // см константы

  /* настройка параметров сна */
  //setSleepMode(STANDBY_SLEEP); // если нужен другой режим сна (по умолчанию POWERDOWN_SLEEP)
  bodInSleep(false); // рекомендуется выключить bod во сне для сохранения энергии (по умолчанию true - включен)

  /* пример однократного ухода в сон */
  Serial.println("go to sleep");
  delay(100); // даем время на отправку
  sleep(SLEEP_2048MS); // спим ~ 2 секунды
  Serial.println("wake up!");
  delay(100); // даем время на отправку

}

void loop() {
  /* пример циклического сна */
  sleepDelay(1500); // спим 1.5 секунды
  digitalWrite(13, !digitalRead(13)); // инвертируем состояние на пине
}