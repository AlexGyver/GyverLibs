// тест отправки и получения на одной ардуине
// перемычка между D2 и D3

#include "GyverBus.h"

GBus<2, GBUS_FULL> tx(1, 10);  // пин 2, адрес 1, буфер 10 байт
GBus<3, GBUS_FULL> rx(2, 10);  // пин 3, адрес 2, буфер 10 байт

// при создании объекта можно указать:
// GBus<пин, GBUS_FULL> - двухсторонняя связь
// GBus<пин, GBUS_TX> - только отправка (экономит память)
// GBus<пин, GBUS_RX> - только приём (экономит память)

// второй параметр - CRC
// GBus<GBUS_FULL, GBUS_NO_CRC>	- отключить ожидание и проверку CRC
// по умолчанию CRC включен

// например структура для отправки
struct myStruct {
  byte val_b;
  int val_i;
  float val_f;
};
myStruct tx_data;

void setup() {
  Serial.begin(9600);

  // заполняем
  tx_data.val_b = 123;
  tx_data.val_i = -23456;
  tx_data.val_f = 3.14;
}

void loop() {
  rx.tick();
  tx.tick();

  // успешный приём
  if (rx.gotData()) {
    // приёмная структура
    myStruct rx_data;

    // читаем в неё
    rx.readData(rx_data);

    // выводим проверяем
    Serial.println(rx_data.val_b);
    Serial.println(rx_data.val_i);
    Serial.println(rx_data.val_f);
    Serial.println();
  }

  // отправляем каждые 2 секунды
  static uint32_t tmr;
  if (millis() - tmr >= 2000) {
    tmr = millis();
    // отправляем на адрес 2
    tx.sendData(2, tx_data);
  }
}
