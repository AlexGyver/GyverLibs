// пример работы с юартом
#include "microUART_print.h"
microUART_print uart;

void setup() {
  uart.begin(9600);
  uart.println("puk");
  uart.println("kek");
}

void loop() {
  if (uart.available()) {      // если есть что на приём
    byte data = uart.read();   // прочитать
    uart.write(data);          // отправить обратно
  }
}
