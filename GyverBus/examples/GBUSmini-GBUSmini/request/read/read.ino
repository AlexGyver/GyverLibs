// принимаем реквест
#include "GBUSmini.h"  // мини-библиотека с лёгкими функциями

#define RX_PIN 4    // пин
#define RX_ADDR 3   // наш адрес

void setup() {
  Serial.begin(9600);

  // ПИН ОБЯЗАТЕЛЬНО PULLUP!!!111
  pinMode(RX_PIN, INPUT_PULLUP);
}

void loop() {
  // ждём приём, ловим адрес
  byte txaddr = GBUS_read_request(RX_PIN, RX_ADDR);
  if (txaddr) {
    Serial.print("Request from ");
    Serial.println(txaddr);
  }
}
