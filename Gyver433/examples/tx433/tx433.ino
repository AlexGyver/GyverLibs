// мелкий передатчик 3.6V SYN115
#define G433_BUFSIZE 50   // размер буфера
#define G433_SPEED 2000   // скорость бит/сек (минимальная)
#include <Gyver433.h>
Gyver433_TX tx(2);  // указали пин

void setup() {
}

char data[] = "Hello from #xx";
byte count = 0;
void loop() {
  data[12] = (count / 10) + '0';
  data[13] = (count % 10) + '0';
  if (++count >= 100) count = 0;
  tx.sendData(data);  
  delay(100);
}
