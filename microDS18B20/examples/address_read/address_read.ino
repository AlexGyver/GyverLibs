#include <microDS18B20.h>
/*
   Чтение уникального адреса термометра для последующего использования
   ВНИМАНИЕ! Устанавливать не более одного датчика на линию (пин), во избежание конфликта
*/

MicroDS18B20 sensor_1(9);           // Создаем термометр без адреса
uint8_t address[8];                 // Создаем массив для адреса

void setup() {
  Serial.begin(9600);
  sensor_1.readAddress(address);    // Читаем адрес термометра в наш массив
  Serial.print("Address: {");
  for (uint8_t i = 0; i < 8; i++) {
    Serial.print("0x");
    Serial.print(address[i], HEX);  // Выводим уникальный адрес для последующего копирования
    if (i < 7)Serial.print(",");
  }
  Serial.print("};");
}

void loop() {}
