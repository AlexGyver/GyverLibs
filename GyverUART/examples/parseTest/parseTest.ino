#include <GyverUART.h>

int dataArray[8];

void setup() {
  uart.begin();    // открыть на 9600
}

void loop() {
  // $10 21 458 63 8 45 875 215;
  if (uart.parsePacket((int*)dataArray)) {
    for (byte i = 0; i < 8; i++) {
      uart.print(dataArray[i]);
      uart.print(" ");
    }
    uart.println();
  }
}
