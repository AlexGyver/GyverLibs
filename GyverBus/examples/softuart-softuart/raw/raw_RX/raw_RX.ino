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
  // в тике сидит отправка и приём
  bus.tick();

  if (bus.gotRaw()) {
    // выводим сырые данные
    for (byte i = 0; i < bus.rawSize(); i++) {
      byte val = bus.buffer[i];
      Serial.print(bus.buffer[i]);
      Serial.print(',');
    }
    Serial.println();
  }
}
