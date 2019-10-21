/* Пример записи и чтения данных в I2C - EEPROM "AT24C32" */

// #include <Wire.h> // заменяем Wire.h на microWire.h
#include <microWire.h>

uint8_t chipAddress = 0x57; // адрес устройства (используйте i2c scaner для определения)
uint16_t cellAddress = 3064; // адрес первой ячейки , куда будем писать и откуда читать
uint8_t data_0 = 115; // данные , которые запишем в EEPROM ( сравнивайте с этим числом при чтении )
uint8_t data_1 = 14;

void setup() {
  Serial.begin(9600);
  Wire.begin();

  /* запись */
  Wire.beginTransmission(chipAddress);  // начинаем передачу с устройством , зовем по адресу
  Wire.write(highByte(cellAddress));    // отправляем старший байт первой адреса ячейки
  Wire.write(lowByte(cellAddress));     // отправляем младший байт первой адреса ячейки
  Wire.write(data_0);                   // отпарвляем байт данных
  Wire.write(data_1);                   // отпарвляем еще байт данных
  Wire.endTransmission();               // завершаем передачу

  delay(50);                            // подождем

  /* чтение */
  Wire.beginTransmission(chipAddress);  // начинаем передачу с устройством , зовем по адресу
  Wire.write(highByte(cellAddress));    // отправляем старший байт адреса  первой ячейки
  Wire.write(lowByte(cellAddress));     // отправляем младший байт адреса  первой ячейки
  Wire.endTransmission();               // завершаем передачу
  Wire.requestFrom(chipAddress , 2);    // запрашиваем свои 2 байта данных
  while (Wire.available()) {            // пока запрошенные данные не кончились
    Serial.println(Wire.read());        // читаем и выводим их
  }

}

void loop() {

}
