// принимаем данные от gbus mini

// подключаем софт юарт
#include "softUART.h"
softUART<4> UART(1000); // пин 4, скорость 1000

// подключаем GBUS
#include "GBUS.h"
GBUS bus(&UART, 3, 20); // обработчик UART, адрес 3, буфер 20 байт

// структура для приёма
struct myStruct {
  byte val_b;
  int val_i;
  float val_f;
};

void setup() {
  Serial.begin(9600); // сериал для отладки (вывод в монитор)
}

void loop() {
  // в тике сидит отправка и приём
  bus.tick();

  if (bus.gotData()) {
    myStruct rxData;
    bus.readData(rxData);
    Serial.println(rxData.val_b);
    Serial.println(rxData.val_i);
    Serial.println(rxData.val_f);
    Serial.println();
  }
}
