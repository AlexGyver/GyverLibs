#include "Hamming.h"
void setup() {
  Serial.begin(9600);

  // создали дату
  char data0[] = "Hello, world! Lorem Ipsum";

  // запаковали
  Hamming<5> buf;   // <> - порядок кода (4-8)
  buf.pack(data0);  // 12мс

  // проебали часть (два байта)
  buf.buffer[5] = 0;
  buf.buffer[6] = 0;

  // распаковываем
  Hamming<5> buf2;
  buf2.unpack(buf.buffer, buf.length());  // 6мс
  Serial.println((char*)buf2.buffer);
  Serial.println(buf2.status());
}

void loop() {
}
