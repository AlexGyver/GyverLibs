// простой пример приёма для simple_send и raw_send
#include "GyverBus.h"

// пин 4, адрес 1, приёмник на 15 байт
GBus<4, GBUS_FULL, GBUS_CRC> rx(1, 15);

void setup() {
  Serial.begin(9600);
}

void loop() {
  static byte lastStatus = 0;
  byte status = rx.tick();
  if (lastStatus != status) {
    Serial.println(status);
    lastStatus = status;
  }
}
