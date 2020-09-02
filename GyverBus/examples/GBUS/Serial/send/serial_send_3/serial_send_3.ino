// более интересный пример
// отправляем данные через аппаратный UART
// ОТПРАВЛЯЕМ СТРУКТУРУ ПО СЕРИАЛ =)
// принимает пример serial_send_read_3
// у этой ардуины провод подключен к пину TX (пин 1)

#define RX_ADDR 3   // адрес приёмника

// структура включает первые три байта протокола GBUS
struct myStruct {
  byte amount;
  byte rxaddr;
  byte txaddr;
  byte val_b;
  int val_i;
  float val_f;
};
myStruct tx_data;

byte buff[sizeof(tx_data)]; // буфер отправки

// подключаем либу для pack функции
#include "GyverBus.h"

void setup() {
  Serial.begin(300);  // скорость настраиваем как в GBUS!
  
  // заполняем буфер, первые три байта по протоколу
  tx_data.amount = sizeof(tx_data);
  tx_data.rxaddr = RX_ADDR;
  tx_data.txaddr = 5;   // наш адрес 5 например

  // заполняем дату
  tx_data.val_b = 123;
  tx_data.val_i = -23456;
  tx_data.val_f = 3.14;

  // пакуем структуру в байтовый буфер
  packDataBytes(buff, tx_data);
}

void loop() {
  // serial write заполняет буфер на отправку по uart
  // то есть вызов не блокирующий, данные будут отправлены
  // полностью асинхронно
  Serial.write(buff, sizeof(buff)); // запускаем  
  delay(2000);
}
