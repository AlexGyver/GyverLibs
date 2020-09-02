// принимаем запрос и отправляем в ответ данные с АЦП

#define RX_PIN 4      // пин
#define RX_ADDR 3     // наш адрес
#include "GBUSmini.h"	// мини-библиотека с лёгкими функциями

void setup() {
  Serial.begin(9600);
}

void loop() {
  // ждём приём на адрес RX_ADDR, т.е. наш адрес RX_ADDR
  byte txaddr = GBUS_read_request(RX_PIN, RX_ADDR);
  if (txaddr) {
    Serial.print("got request from ");
    Serial.println(txaddr);
    GBUS_send_ack(RX_PIN, txaddr, RX_ADDR);
  }
}
