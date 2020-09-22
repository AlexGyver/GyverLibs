// пакуем данные любого типа (например, структура) в буфер и отправляем
// принимаем на softuart (под GBUS) и распаковываем
#define GBUS_SPEED 1000
#include "GBUSmini.h"	// мини-библиотека с лёгкими функциями

#define TX_PIN 4    // пин
#define RX_ADDR 3   // адрес приёмника
#define TX_ADDR 5   // наш адрес

// структура для отправки
struct myStruct {
  byte val_b;
  int val_i;
  float val_f;
};
myStruct txData;              // отправная структура
byte buffer[sizeof(txData)];  // отправной буфер

void setup() {
  // ПИН ОБЯЗАТЕЛЬНО PULLUP!!!111
  pinMode(TX_PIN, INPUT_PULLUP);

  // заполняем структуру
  txData.val_b = 123;
  txData.val_i = -23456;
  txData.val_f = 3.14;

  // пакуем структуру в буфер
  packDataBytes(buffer, txData);
}

void loop() {
  // пин, адрес получателя, адрес отправителя, дата, размер
  GBUS_send(TX_PIN, RX_ADDR, TX_ADDR, buffer, sizeof(buffer));
  delay(2000);
}
