/*
   Пример индивидуальных настроек датчика под ваше применение
   См. константы в GyverBME280.h , стандартные настройки можно изменить там же в классе GyverMBE280
   Настройки вступают в силу только ПОСЛЕ .begin();
*/

#include <GyverBME280.h>                            // Подключение библиотеки
GyverBME280 bme;                                    // Создание обьекта bme

void setup() {
  Serial.begin(9600);                               // Запуск последовательного порта  
  bme.setFilter(FILTER_COEF_8);                     // Настраиваем коофициент фильтрации
  bme.setTempOversampling(OVERSAMPLING_8);          // Настраиваем передискретизацию для датчика температуры
  bme.setPressOversampling(OVERSAMPLING_16);        // Настраиваем передискретизацию для датчика давления
  bme.setStandbyTime(STANDBY_500MS);                // Устанавливаем время сна между измерениями (у нас обычный циклический режим)
  bme.begin();                                      // Если на этом настройки окончены - инициализируем датчик
}

void loop() {
  Serial.print("Temperature: ");
  Serial.print(bme.readTemperature());              // Выводим темперутуру в [*C]
  Serial.println(" *C");

  Serial.print("Humidity: ");
  Serial.print(bme.readHumidity());                 // Выводим влажность в [%]
  Serial.println(" %");

  Serial.print("Pressure: ");
  Serial.print(pressureToMmHg(bme.readPressure())); // Выводим давление в мм рт. столба
  Serial.println(" mm Hg");
  Serial.println("");
  delay(1000);
}
