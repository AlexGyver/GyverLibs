// отправка сырых данных по однопроводному юарту

// подключаем софт юарт
#include "softUART.h"
softUART<4> UART(1000); // пин 4, скорость 1000

// подключаем GBUS
#include "GBUS.h"
GBUS bus(&UART, 3, 20); // обработчик UART, адрес 3, буфер 20 байт

void setup() {  
  
}


void loop() {
  // в тике сидит отправка и приём
  bus.tick();

  static uint32_t tmr;
  if (millis() - tmr >= 2000) {
    tmr = millis();
    // отправляем массив каждые 2 секунды
    byte buf[] = {1, 2, 3, 4, 5};
    bus.sendRaw(buf, sizeof(buf));    
  }
}
