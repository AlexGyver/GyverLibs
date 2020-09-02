// тест реквеста на одной ардуине
// перемычка между D2 и D3

#include "GyverBus.h"

GBus<2, GBUS_FULL> tx(1, 10);  // пин 2, адрес 1, буфер 10 байт
GBus<3, GBUS_FULL> rx(2, 10);  // пин 3, адрес 2, буфер 10 байт

void setup() {
  Serial.begin(9600);
}

void loop() {
  rx.tick();
  tx.tick();

  // получили запрос
  if (rx.gotRequest()) {
    Serial.print("request from ");
    Serial.print(rx.getTXaddress());
    Serial.println();
  }

  static uint32_t tmr;
  if (millis() - tmr > 2000) {
    tmr = millis();
    // отправляем запрос на адрес 2
    tx.sendRequest(2);
  }
}
