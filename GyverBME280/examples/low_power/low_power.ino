/*
   Пример работы датчика с пониженным энергопотреблением
   По умолчанию - NORMAL_MODE - Время сна между преобразованиями - 250мс
   См. константы в GyverBME280.h , стандартные настройки можно изменить там же в классе GyverMBE280
   Настройки вступают в силу только ПОСЛЕ .begin();
*/

#include <GyverBME280.h>                            // Подключение библиотеки
GyverBME280 bme;                                    // Создание обьекта bme

void setup() {
  Serial.begin(9600);                               // Запуск последовательного порта
  bme.setHumOversampling(MODULE_DISABLE);           // Отключаем неиспользуемый модуль измерения влажности - экономим энергию
  // bme.setMode(FORCED_MODE);                      // По возможности используем принудительный режим с редким опросом
  bme.setStandbyTime(STANDBY_1000MS);               // Если используем обычный режим - увеличиваем время сна между измерениями насколько это возможно в нашем случае
  bme.begin();                                      // Если на этом настройки окончены - инициализируем датчик
}

void loop() {
  // bme.oneMeasurement();                          // Если используем принудительный мод - необходимо будить датчик для проведения измерения
  // while (bme.isMeasuring());                     // И дождаться окончания текущего измерения , чтобы не взять устаревшие данные
  Serial.print("Temperature: ");
  Serial.print(bme.readTemperature());              // Выводим темперутуру в [*C]
  Serial.println(" *C");

  Serial.print("Pressure: ");
  Serial.print(pressureToMmHg(bme.readPressure())); // Выводим давление в мм рт. столба
  Serial.println(" mm Hg");
  Serial.println("");
  delay(1000);
}
