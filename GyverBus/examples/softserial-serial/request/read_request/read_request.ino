// принимаем запрос через SoftwareSerial
// здесь провод подключен к ноге RX софт сериала
// у меня настроено на D10

#include <SoftwareSerial.h>
SoftwareSerial mySerial(10, 11); // RX, TX
// сначала объявляем обработчик
// это может почти быть любая интерфейсная либа,
// например софтСериал на любой другой платформе

#include "GBUS.h"
GBUS bus(&mySerial, 3, 20);  // адрес 3, буфер 10 байт

void setup() {
  // родной сериал открываю для наблюдения за процессом
  Serial.begin(9600);

  // этот сериал будет принимать данные
  mySerial.begin(9600);
}

void loop() {
  // здесь принимаются данные
  // если это аппаратный сериал - слишком часто его опрашивать даже не нужно
  bus.tick();

  // приняли данные
  if (bus.gotRequest()) {
    Serial.print("request from: ");
    Serial.println(bus.getTXaddress());
  }
}
