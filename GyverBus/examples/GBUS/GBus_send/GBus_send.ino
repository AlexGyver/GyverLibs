// простой пример отправки (принимает пример GBus_read)
#include "GyverBus.h"

#define TX_PIN 4    // пин
#define RX_ADDR 3   // адрес приёмника
#define TX_ADDR 5   // наш адрес

// пин 4, адрес 1, буфер на 15 байт
GBus<TX_PIN, GBUS_TX> tx(TX_ADDR, 15);

void setup() {
  Serial.begin(9600);
}

// данные
int data = 12345;

void loop() {
  if (tx.tick() == TX_COMPLETE) Serial.println("transmitted");

  static uint32_t tmr;
  if (millis() - tmr >= 1000) {
    tmr = millis();
    tx.sendData(RX_ADDR, data);
  }
}
