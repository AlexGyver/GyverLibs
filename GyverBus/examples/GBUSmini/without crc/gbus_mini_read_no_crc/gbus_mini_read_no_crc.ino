// пример приёма данных
// отключаем CRC
#define GBUS_MINI_NO_CRC

#include "GBUSmini.h"  // мини-библиотека с лёгкими функциями

#define RX_PIN 4    // пин
#define RX_ADDR 3   // наш адрес
byte data[6];       // приёмный буфер (байты)

void setup() {
  Serial.begin(9600);

  // ПИН ОБЯЗАТЕЛЬНО PULLUP!!!111
  pinMode(RX_PIN, INPUT_PULLUP);
}

void loop() {
  // ждём приём, ловим адрес
  byte txaddr = GBUS_read(RX_PIN, RX_ADDR, data, sizeof(data));
  if (txaddr) {
    // если успешно приняли
    for (byte i = 0; i < sizeof(data); i++) {
      Serial.print(data[i]);
      Serial.print(", ");
    }
    Serial.print(" from address ");
    Serial.println(txaddr);
  }
}
