// отправляем структуру через Serial
// здесь провод подключен к ноге TX (D1)

#include "GBUS.h"
// адрес 5, буфер 20 байт
GBUS bus(&Serial, 5, 20);  

// структура для отправки
struct myStruct {
  byte val_b;
  int val_i;
  long val_l;
  float val_f;
};

void setup() {
  // запускаем интерфейс
  Serial.begin(9600);
}

void loop() {  
  myStruct data;
  data.val_b = 12;
  data.val_i = 1234;
  data.val_l = 123456;
  data.val_f = 123.45678;

  // отправляем на адрес 3
  bus.sendData(3, data);
  delay(2000);

  // tick() тут не нужен! Он занимается только приёмом данных
  // отправка делается так, как реализовано в используемой либе интерфейса
}
