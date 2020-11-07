// отправка данных по однопроводному юарту

// подключаем софт юарт
#include "softUART.h"
// делаем только отправителем (экономит память)
softUART<4, GBUS_TX> UART(1000); // пин 4, скорость 1000

// подключаем GBUS
#include "GBUS.h"
GBUS bus(&UART, 3, 20); // обработчик UART, адрес 3, буфер 20 байт

struct myStruct {
  byte val_b;
  int val_i;
  long val_l;
  float val_f;
};

void setup() {
}


void loop() {
  // в тике сидит отправка и приём
  bus.tick();

  static uint32_t tmr;
  if (millis() - tmr >= 2000) {
    tmr = millis();
    // отправляем каждые 2 секунды
    myStruct data;
    data.val_b = 12;
    data.val_i = 1234;
    data.val_l = 123456;
    data.val_f = 123.45678;
    bus.sendData(5, data);  // на адрес 5
  }
}
