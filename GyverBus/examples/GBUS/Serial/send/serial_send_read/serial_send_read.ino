// принимаем данные из примера serial_send_read
// у этой ардуины провод подключен к пину RX_PIN
#include "GyverBus.h"

#define RX_PIN 4    // пин
#define RX_ADDR 3   // наш адрес

// пин 4, адрес 3, приёмник на 15 байт
GBus<RX_PIN, GBUS_RX, GBUS_NO_CRC> rx(RX_ADDR, 15);

void setup() {
  Serial.begin(9600);
}

void loop() {
  rx.tick();

  if (rx.gotData()) {
    byte in;
    rx.readData(in);
    Serial.println(in);
  }
}
