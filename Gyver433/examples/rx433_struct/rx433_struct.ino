// передача структуры данных
// крупный приёмник 5.0V SYN480R
#define G433_BUFSIZE 50   // размер буфера
#define G433_SPEED 2000   // скорость бит/сек (минимальная)

#include <Gyver433.h>
Gyver433_RX rx(2);  // указали пин

// формат пакета для приёма (такой же как отправляется)
struct dataPack {
  byte counter;
  byte randomNum;
  int analog;
  uint32_t time;
};

void setup() {
  Serial.begin(9600);
}

void loop() {
  // tick принимает асинхронно, но может ловить ошибки при загруженном коде
  // tickWait блокирует выполнение, но принимает данные чётко
  if (rx.tick()) {
    dataPack data;
    rx.readData(data); // прочитать в buf

    Serial.println("Received:");
    Serial.println(data.counter);
    Serial.println(data.randomNum);
    Serial.println(data.analog);
    Serial.println(data.time);
    Serial.println();
  }
}
