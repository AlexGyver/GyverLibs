// приём сырых данных по однопроводному юарту

// подключаем софт юарт
#include "softUART.h"
softUART<4> UART(1000); // пин 4, скорость 1000

// подключаем GBUS
#include "GBUS.h"
GBUS bus(&UART, 5, 20); // обработчик UART, адрес 5, буфер 20 байт

void setup() {
  Serial.begin(9600); // сериал для отладки (вывод в монитор)
}

void loop() {
  bus.tick();   // тикаем
  if (bus.statusChanged()) {  // если статус изменился
    Serial.println(bus.getStatus());  // выводим код
  }
}
