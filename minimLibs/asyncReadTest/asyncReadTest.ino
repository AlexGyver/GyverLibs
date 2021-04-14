// асинхронная читалка Serial
// а также любых из класса Stream

#include "AsyncStream.h"

// указываем обработчик, терминатор (символ конца приёма) и таймаут в мс
// в <> указан размер буфера!
AsyncStream<100> serial(&Serial, '\n');

void setup() {
  Serial.begin(9600);
}

// строка для теста (отправь в сериал)
// 1234,3.14,hello,4567,lolkek,qwerty
void loop() {
  if (serial.available()) {     // если данные получены
    Serial.println(serial.buf); // выводим их (как char*)    
  }
}
