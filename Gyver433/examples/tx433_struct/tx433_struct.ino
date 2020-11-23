// передача структуры данных
// мелкий передатчик 3.6V SYN115

#define G433_BUFSIZE 50   // размер буфера
#define G433_SPEED 2000   // скорость бит/сек (минимальная)
#include <Gyver433.h>
Gyver433_TX tx(2);  // указали пин

// формат пакета для отправки
struct dataPack {
  byte counter;
  byte randomNum;
  int analog;
  uint32_t time;
};
dataPack data;

void setup() {
  Serial.begin(9600);
}

void loop() {
  data.counter++;                 // тут счётчик
  data.randomNum = random(256);   // случайное число
  data.analog = analogRead(0);    // тут ацп
  data.time = millis();           // тут миллис

  Serial.println("Transmit:");
  Serial.println(data.counter);
  Serial.println(data.randomNum);
  Serial.println(data.analog);
  Serial.println(data.time);
  Serial.println();

  tx.sendData(data);
  delay(1000);
}
