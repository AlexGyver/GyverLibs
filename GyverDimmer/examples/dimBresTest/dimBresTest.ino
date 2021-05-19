// тест одного канала по Брезенхему

#define D_PIN 5
// zero cross на D2

#include <GyverDimmer.h>
DimmerBres<D_PIN> dim;   // указать пин диммера

void setup() {
  // завести прерывание на детектор нуля
  attachInterrupt(0, isr, RISING);  // D2 == 0
}

void isr() {
  dim.tick(); // вызывать тик в прерывании детектора нуля
}

void loop() {
  // принимает 0-255
  dim.write(analogRead(A0) / 4);
  delay(100);
}
