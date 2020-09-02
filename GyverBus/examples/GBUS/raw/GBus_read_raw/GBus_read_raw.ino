// простой пример приёма сырых данных по шине
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

  if (rx.gotRaw()) {
    for (byte i = 0; i < rx.rawSize(); i++) {
      Serial.print(rx.buffer[i]);
      Serial.print(',');
    }
    Serial.println();
  }
}
