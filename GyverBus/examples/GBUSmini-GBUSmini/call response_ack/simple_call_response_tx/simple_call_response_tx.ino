// отправляем запрос с подтверждением

#define TX_PIN 4      // пин
#define RX_ADDR 3     // адрес приёмника
#define TX_ADDR 5     // наш адрес
#include "GBUSmini.h"	// мини-библиотека с лёгкими функциями

void setup() {
  Serial.begin(9600);
}

void loop() {
  // отправляем запрос каждые три секунды
  static uint32_t tmr;
  if (millis() - tmr > 3000) {
    tmr = millis();
    // отправлять запросы и ждать подтверждения приёма, т.е. пытаться достучаться
    // (пин, адрес получателя, адрес отправителя, кол-во попыток, таймаут между попытками)
    if (GBUS_send_request_ack(TX_PIN, RX_ADDR, TX_ADDR, 3, 500)) {
      Serial.println("got ack");
    } else {
      Serial.println("error");
    }
  }
}
