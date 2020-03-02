#include <microDS18B20.h>
/*
   Пример нескольких термометров на одном пине с использованием адресации
   Использование адресации для нескольких датчиков значительно экономит память
   ВНИМАНИЕ! Температура запрашивается для ОДНОГО датчика на одной линии (пине)
   Пока идет преобразование температуры линия "занята"
   Для паралельного считывания температуры используйте несколько линий 
*/

#define DS_PIN 9 // пин для термометров

/*
   Уникальные адреса датчиков - считать можно в примере address_read
*/
const uint8_t sensor1_address[8] = {0x28, 0x13, 0xCD, 0xEC, 0x9, 0x0, 0x0, 0x7};
const uint8_t sensor2_address[8] = {0x32, 0x10, 0xC0, 0x1C, 0x90, 0x0, 0x0, 0x9};

MicroDS18B20 sensor1(DS_PIN, sensor1_address);  // Создаем термометр с адресацией
MicroDS18B20 sensor2(DS_PIN, sensor2_address);  // Создаем термометр с адресацией

void setup() {
  Serial.begin(9600);
}

void loop() {
  sensor1.requestTemp();             // Запрашиваем преобразование температуры
  delay(1000);                              // Ожидаем окончания преобразования
  Serial.print("Sensor1 temp: ");
  Serial.print(sensor1.getTemp());  // Выводим температуру
  Serial.println(" *C");

  sensor2.requestTemp();
  delay(1000);
  Serial.print("Sensor2 temp: ");
  Serial.print(sensor2.getTemp());
  Serial.println(" *C");
}
