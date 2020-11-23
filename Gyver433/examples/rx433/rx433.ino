// крупный приёмник 5.0 SYN480R

#define G433_BUFSIZE 50   // размер буфера
#define G433_SPEED 2000   // скорость бит/сек (минимальная)

#include <Gyver433.h>
Gyver433_RX rx(2);

void setup() {
  Serial.begin(9600);
}


void loop() {
  // tick принимает асинхронно, но может ловить ошибки при загруженном коде
  // tickWait блокирует выполнение, но принимает данные чётко
  if (rx.tickWait()) {
    byte buf[64];
    rx.readData(buf);
    for (byte i = 0; i < rx.size; i++) Serial.write(buf[i]);
  }
}
