// тест нескольких диммеров по Брезенхему
// zero cross на D2

#include <GyverDimmer.h>
DimmerBresMulti<2> dim;   // указать количество диммеров

void setup() {
  // завести общее прерывание на детектор нуля
  attachInterrupt(0, isr, RISING);  // D2 == 0

  // подключить пины
  dim.attach(0, 4);   // канал 0, пин 4
  dim.attach(1, 5);   // канал 1, пин 5
}

void isr() {
  dim.tick(); // вызывать тик в прерывании детектора нуля
}

void loop() {
  byte val = analogRead(A0) / 4;
  // врубаем, принимает 0-255
  dim.write(0, val);        // канал 0
  dim.write(1, 255 - val);  // канал 1
  delay(100);
}
