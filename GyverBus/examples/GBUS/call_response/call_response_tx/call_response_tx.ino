// отправляем запрос и в ответ ждём данные, т.е.
// отправитель работает на отправку, потом на приём
// парный пример - call_response_tx

#include "GyverBus.h"

#define TX_PIN 4    // пин
#define RX_ADDR 3   // адрес приёмника
#define TX_ADDR 5   // наш адрес

GBus<TX_PIN, GBUS_FULL> tx(TX_ADDR, 15);

void setup() {
  Serial.begin(9600);
}

void loop() {
  tx.tick();
  
  // отправляем запрос на RX_ADDR каждые три секунды
  static uint32_t tmr;
  if (millis() - tmr > 3000) {
    tmr = millis();
    tx.sendRequest(RX_ADDR);
    Serial.println("send request");
  }

  // если нам что-то отправили
  if (tx.gotData()) {
    int data;
    // читаем и выводим
    tx.readData(data);
    Serial.print("received ");
    Serial.println(data);
  }
}
