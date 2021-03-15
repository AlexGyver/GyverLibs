// пример работы с юартом
#include "microUART_ISR.h"
microUART_ISR uart;

void setup() {
  uart.begin(9600);
  uart.write(40);  // отправить байт 40
  uart.write(40);
}

void loop() {
  while (uart.available()) {      // если есть что на приём
    byte data = uart.read();   // прочитать
    uart.write(data);          // отправить обратно
  }
  delay(200);
}
