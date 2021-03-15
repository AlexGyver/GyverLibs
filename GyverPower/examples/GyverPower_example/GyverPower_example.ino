// демо возможностей библиотеки

#include <GyverPower.h>

void setup() {
  pinMode(13, OUTPUT); // настраиваем вывод со светодиодом на выход
  Serial.begin(9600);

  // калибровка таймаутов для максимальной точности sleepDelay (подробнее в примере WDT_calibration)
  //Serial.println(getMaxTimeout());  // вывести реальный макс. период
  //calibrate(8935); 			            // ввести реальный макс. период
  //calibrate(getMaxTimeout());       // автоматически посчитать и откалибровать
  
  power.autoCalibrate(); // автоматическая калибровка ~ 2 секунды , средняя но достаточная точность

  // отключение ненужной периферии
  power.hardwareDisable(PWR_ADC | PWR_TIMER1); // см раздел константы в GyverPower.h, разделяющий знак " | "

  // управление системной частотой
  power.setSystemPrescaler(PRESCALER_2); // см константы в GyverPower.h
  
  // настройка параметров сна
  power.setSleepMode(STANDBY_SLEEP); // если нужен другой режим сна, см константы в GyverPower.h (по умолчанию POWERDOWN_SLEEP)
  power.bodInSleep(false); // рекомендуется выключить bod во сне для сохранения энергии (по умолчанию false - выключен!!)

  // пример однократного ухода в сон
  Serial.println("go to sleep");
  delay(100); // даем время на отправку
  
  power.sleep(SLEEP_2048MS); // спим ~ 2 секунды
  
  Serial.println("wake up!");
  delay(100); // даем время на отправку
}

void loop() {
  // пример циклического сна
  power.sleepDelay(1500); // спим 1.5 секунды
  
  digitalWrite(13, !digitalRead(13)); // инвертируем состояние на пине
}
