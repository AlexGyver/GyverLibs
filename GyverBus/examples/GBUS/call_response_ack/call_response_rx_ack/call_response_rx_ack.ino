// принимаем запрос и отправляем в ответ данные с АЦП или ack
// парный пример - call_response_rx_ack

#include "GyverBus.h"

#define RX_PIN 4    // пин
#define RX_ADDR 3   // наш адрес

// работает как отправитель и приёмник!! GBUS_FULL
GBus<RX_PIN, GBUS_FULL> rx(RX_ADDR, 15);

void setup() {
  Serial.begin(9600);
}

void loop() {
  rx.tick();

  // приняли запрос request
  if (rx.gotRequest()) {
    Serial.print("ack from ");
    Serial.print(rx.getTXaddress());
    Serial.println();

    // прочитали аналог
    int val = analogRead(0);
    Serial.print("sending ");
    Serial.println(val);

    // отправили обратно
    rx.sendData(rx.getTXaddress(), val);  // отправить дату
    //rx.sendAck(rx.getTXaddress());  // отправить в ответ ack
  }
}
