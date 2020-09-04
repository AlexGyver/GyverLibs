#include "GyverBus.h"

void setup() {
  Serial.begin(9600);
  
  // любой массив байт
  byte bytes[] = {0, 1, 2, 3, 4, 5, 6, 7, 8, 9};

  // считаем CRC всех кроме последнего
  byte crc = GBUS_crc_bytes(bytes, sizeof(bytes) - 1);

  // наш CRC
  Serial.print("CRC: ");
  Serial.println(crc);

  // присвоим к последнему элементу массива
  bytes[9] = crc;

  // если скормить функции все байты + их CRC в конце 
  // - получится 0
  Serial.print("CRC recursive: ");
  Serial.println(GBUS_crc_bytes(bytes, sizeof(bytes)));
}

void loop() {
  // put your main code here, to run repeatedly:

}
