// отправляем запрос через SoftwareSerial
// ожидаем ответа
// связь двухсторонняя!!!! Это скетч ПЕРЕДАТЧИКА
// TX передатчика подключен к RX приёмника
// RX передатчика подключен к TX приёмника

#include <SoftwareSerial.h>
SoftwareSerial mySerial(10, 11); // RX, TX
// сначала объявляем обработчик
// это может почти быть любая интерфейсная либа,
// например софтСериал на любой другой платформе

// указываем "обработчик" интерфейса перед подключением библиотеки
#include "GBUS.h"

// адрес 5, буфер 5 байт, скорость 9600 бод
GBUS bus(&mySerial, 5, 5);

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

  // отправляем запрос на адрес 3 каждые 5 секунд
  static uint32_t tmr;
  if (millis() - tmr > 5000) {
    tmr = millis();
    Serial.println("send request");
    bus.sendRequest(3);
  }

  // ждём ответа от 3
  // пытаемся достучаться через таймаут 500мс 3 раза
  byte state = bus.waitAck(3, 3, 500);
  switch (state) {
    case ACK_IDLE: //Serial.println("idle");
      break;
    case ACK_WAIT: //Serial.println("wait");
      break;
    case ACK_ERROR: Serial.println("ack error");
      break;
    case ACK_ONLY: Serial.println("got ack");
      break;
    case ACK_DATA: Serial.print("got data: ");
      // читаем и выводим
      byte val;
      bus.readData(val);
      Serial.println(val);
      break;
  }
}
