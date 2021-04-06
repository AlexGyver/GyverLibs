#include "RingBuffer.h"
RingBuffer<int, 16> buf;

void setup() {
  Serial.begin(9600);
  buf.write(12);
  buf.write(34);
  buf.write(56);
  Serial.println(buf.available());
  while (buf.available()) {
    Serial.println(buf.read());
  }
}

void loop() {
}
