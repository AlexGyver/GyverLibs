// принимаем запрос и отправляем в ответ данные с АЦП
// парный пример - call_response_rx

#include "GyverBus.h"

#define RX_PIN 4    // пин
#define RX_ADDR 3   // наш адрес

GBus<RX_PIN, GBUS_FULL> rx(RX_ADDR, 15);

void setup() {
  Serial.begin(9600);
}

void loop() {
  rx.tick();

  // приняли запрос
  if (rx.gotRequest()) {
    Serial.print("request from ");
    Serial.print(rx.getTXaddress());
    Serial.println();

    // прочитали аналог
    int val = analogRead(0);
    Serial.print("sending ");
    Serial.println(val);

    // отправили обратно
    rx.sendData(rx.getTXaddress(), val);
  }
}
