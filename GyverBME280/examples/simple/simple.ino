/*
   Простой пример, демонстрирующий основные функции измерения температуры, давления и влажности
*/

#include <GyverBME280.h>                      // Подключение библиотеки
GyverBME280 bme;                              // Создание обьекта bme

void setup() {
  Serial.begin(9600);                         // Запуск последовательного порта
  bme.begin();                                // Если доп. настройки не нужны  - инициализируем датчик
}

void loop() {
  Serial.print("Temperature: ");
  Serial.print(bme.readTemperature());        // Выводим темперутуру в [*C]
  Serial.println(" *C");

  Serial.print("Humidity: ");
  Serial.print(bme.readHumidity());           // Выводим влажность в [%]
  Serial.println(" %");

  float pressure = bme.readPressure();        // Читаем давление в [Па]
  Serial.print("Pressure: ");
  Serial.print(pressure / 100.0F);            // Выводим давление в [гПа]
  Serial.print(" hPa , ");
  Serial.print(pressureToMmHg(pressure));     // Выводим давление в [мм рт. столба]
  Serial.println(" mm Hg");
  Serial.print("Altitide: ");
  Serial.print(pressureToAltitude(pressure)); // Выводим высоту в [м над ур. моря]
  Serial.println(" m");
  Serial.println("");
  delay(1000);
}
