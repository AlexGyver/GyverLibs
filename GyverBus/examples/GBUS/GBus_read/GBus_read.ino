// простой пример приёма (вотправляет пример GBus_send)
#include "GyverBus.h"

#define RX_PIN 4    // пин
#define RX_ADDR 3   // наш адрес

// пин 4, адрес 3, приёмник на 15 байт
GBus<RX_PIN, GBUS_RX> rx(RX_ADDR, 15);

void setup() {
  Serial.begin(9600);
}

void loop() {
  rx.tick();

  if (rx.gotData()) {
    int rec;
    rx.readData(rec);
    Serial.println(rec);
  }
}
