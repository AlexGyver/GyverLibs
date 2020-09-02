// отправляем запрос и в ответ ждём данные или подтверждение ack
// пытаемся отправить несколько раз и ждём таймаут
// парный пример - call_response_tx_ack

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
  // отправляем запрос на RX_ADDR каждые три секунды
  static uint32_t tmr;
  if (millis() - tmr > 3000) {
    // отправить запрос и ждать подтверждения
    // (адрес, количество попыток, таймаут между ними)
    byte state = tx.sendRequestAck(RX_ADDR, 3, 1000);
    switch (state) {
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
    tmr = millis();
  }

  /*// если нам что-то отправили
    if (tx.gotData()) {
    int data;
    // читаем и выводим
    tx.readData(data);
    Serial.print("received ");
    Serial.println(data);
    }*/
}
