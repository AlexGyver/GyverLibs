// периодично шлём свои данные. Шлём только тогда, когда линия не занята

#define BUS_PIN 4   // пин
#define TX_ADDR 5   // наш адрес (мы отправитель)
#define RX_ADDR 3   // адрес приёмника

#include "GBUSmini.h"	// мини-библиотека с лёгкими функциями

void setup() {
}

byte data[1] = {0};
void loop() {
  // тупо ждём, пока линия освободится
  while(GBUS_is_busy(BUS_PIN));

  GBUS_send(BUS_PIN, RX_ADDR, TX_ADDR, data, sizeof(data));
  data[0]++;
  delay(2000);

  // либо используем так
  // if (!GBUS_is_busy(BUS_PIN)) GBUS_send(BUS_PIN, RX_ADDR, TX_ADDR, data, sizeof(data));
  // т.е. если линия свободна - отправить
}
