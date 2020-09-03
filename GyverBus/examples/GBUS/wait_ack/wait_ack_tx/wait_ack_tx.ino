// отправляем запрос и в ответ ждём данные или подтверждение ack
// пытаемся отправить несколько раз и ждём таймаут
// парный пример - wait_ack_rx

#include "GyverBus.h"

#define TX_PIN 4    // пин
#define RX_ADDR 3   // адрес приёмника
#define TX_ADDR 5   // наш адрес

// работает как отправитель и приёмник!! GBUS_FULL
GBus<TX_PIN, GBUS_FULL> tx(TX_ADDR, 15);

void setup() {
  Serial.begin(9600);
}

void loop() {
  tx.tick();

  // отправляем запрос на RX_ADDR каждые 5 секунд
  static uint32_t tmr;
  if (millis() - tmr > 5000) {
    tmr = millis();
    Serial.println("send request");
    tx.sendRequest(RX_ADDR);
  }

  // ждём ответа от RX_ADDR
  // пытаемся достучаться через таймаут 500мс 3 раза
  byte state = tx.waitAck(RX_ADDR, 3, 500);
  switch (state) {
    case ACK_IDLE: //Serial.println("idle");
      break;
    case ACK_WAIT: //Serial.println("wait");
      break;
    case ACK_ERROR: Serial.println("ack error");
      break;
    case ACK_ONLY: Serial.println("got ack");
      break;
    case ACK_DATA: Serial.print("got data: ");
      int val;
      tx.readData(val);
      Serial.println(val);
      break;
  }
}
