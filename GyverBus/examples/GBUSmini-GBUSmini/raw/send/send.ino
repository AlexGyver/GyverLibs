// простая функция отправки по GBUS с контролем целостности данных
#include "GBUSmini.h"	// мини-библиотека с лёгкими функциями

#define TX_PIN 4    // пин

void setup() {
  // ПИН ОБЯЗАТЕЛЬНО PULLUP!!!111
  pinMode(TX_PIN, INPUT_PULLUP);
}

// отправляемая дата (байты)
byte data[] = {12, 34, 56};

void loop() {
  GBUS_send_raw(TX_PIN, data, sizeof(data));
  delay(1000);
}
