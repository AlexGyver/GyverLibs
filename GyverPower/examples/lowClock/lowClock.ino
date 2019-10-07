#define FIX_TIME // Коррекция времени в случае изменения системной частоты
// дефайнить до подключения библиотеки!!!

#include <GyverPower.h>

void setup() {
  setSystemPrescaler(PRESCALER_16); // замедляем в 16 раз
  // чтобы сериал завёлся (если нужен), умножаем скорость на замедление
  Serial.begin(9600 * 16L);
  // иначе не заведётся на указанной скорости

  Serial.println("serial test");
  pinMode(13, OUTPUT);
}

void loop() {
  digitalWrite(13, !digitalRead(13)); // мигаем
  delay(1000);    // держит 1 секунду, несмотря на пониженный клок!
}
