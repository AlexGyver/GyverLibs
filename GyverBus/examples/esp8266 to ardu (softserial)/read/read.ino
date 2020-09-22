// принимаем структуру через SoftwareSerial
// отправляет esp8266, принимает Ардуина (этот скетч)
// здесь провод подключен к ноге RX софт сериала
// Провод подключен к D2 ноды и D10 Ардуины

#include <SoftwareSerial.h>
SoftwareSerial mySerial(10, 11); // RX, TX
// сначала объявляем обработчик
// это может почти быть любая интерфейсная либа,
// например софтСериал на любой другой платформе

#include "GBUS.h"
GBUS bus(&mySerial, 3, 25);  // адрес 3, буфер 25 байт

// приёмная структура
// структура структур должна быть одинаковая
struct myStruct {
  float val_f;
  float val_f2;
  long val_i;   // а тут long, иначе всё сломается
  long val_l;
  byte val_b;
};

void setup() {
  // родной сериал открываю для наблюдения за процессом
  Serial.begin(9600);

  // этот сериал будет принимать данные
  // мы указали его как обработчик
  mySerial.begin(9600);
}

void loop() {
  // здесь принимаются данные
  // если это аппаратный сериал - слишком часто его опрашивать даже не нужно
  bus.tick();

  // приняли данные
  if (bus.gotData()) {
    myStruct data;
    bus.readData(data);

    // выводим
    Serial.println(data.val_f);
    Serial.println(data.val_f2);
    Serial.println(data.val_i);
    Serial.println(data.val_l);
    Serial.println(data.val_b);
    Serial.println();
  }
}
