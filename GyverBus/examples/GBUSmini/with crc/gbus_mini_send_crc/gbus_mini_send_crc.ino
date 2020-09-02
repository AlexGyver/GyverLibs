// простая функция отправки по GBUS с контролем целостности данных
#include "GBUSmini.h"	// мини-библиотека с лёгкими функциями

#define TX_PIN 4    // пин
#define RX_ADDR 3   // адрес приёмника
#define TX_ADDR 5   // наш адрес

void setup() {
  // ПИН ОБЯЗАТЕЛЬНО PULLUP!!!111
  pinMode(TX_PIN, INPUT_PULLUP);
}

// отправляемая дата (байты)
byte data[] = {12, 34, 56};

void loop() {
  // пин, адрес получателя, адрес отправителя, дата, размер
  GBUS_send_crc(TX_PIN, RX_ADDR, TX_ADDR, data, sizeof(data));
  delay(1000);
}
