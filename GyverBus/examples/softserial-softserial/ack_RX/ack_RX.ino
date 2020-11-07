// принимаем запрос через SoftwareSerial
// отвечаем на запрос
// связь двухсторонняя!!!! Это скетч ПРИЁМНИКА
// TX передатчика подключен к RX приёмника
// RX передатчика подключен к TX приёмника

#include <SoftwareSerial.h>
SoftwareSerial mySerial(10, 11); // RX, TX
// сначала объявляем обработчик
// это может почти быть любая интерфейсная либа,
// например софтСериал на любой другой платформе

// указываем "обработчик" интерфейса перед подключением библиотеки
#include "GBUS.h"

GBUS bus(&mySerial, 3, 5);  // адрес 3, буфер 5 байт

void setup() {
  // родной сериал открываю для наблюдения за процессом
  Serial.begin(9600);

  // этот сериал будет принимать и отправлять данные
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

    // ответили
    //bus.sendAck(bus.getTXaddress());

    // ИЛИ отправили дату (случайный байт) в ответ
    byte data = random(255);
    Serial.print("sending: ");
    Serial.println(data);
    bus.sendData(bus.getTXaddress(), data);
  }
}
