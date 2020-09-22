// отправляем запрос и в ответ ждём данные с АЦП

#define TX_PIN 4      // пин
#define RX_ADDR 3     // адрес приёмника
#define TX_ADDR 5     // наш адрес
#include "GBUSmini.h"	// мини-библиотека с лёгкими функциями

byte buf[2];

void setup() {
  Serial.begin(9600);
}

void loop() {
  // отправляем запрос каждые три секунды
  static uint32_t tmr;
  if (millis() - tmr > 3000) {
    tmr = millis();
    GBUS_send_request(TX_PIN, RX_ADDR, TX_ADDR);
  }

  if (GBUS_read(TX_PIN, TX_ADDR, buf, sizeof(buf))) {
    // склеиваем байты обратно в инт
    int val = buf[0] | (buf[1] << 8);
    Serial.print("received ");
    Serial.println(val);
  }
}
