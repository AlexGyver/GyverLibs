// простой пример приёма сырых данных и их разбор вручную
#include "GBUSmini.h"  // мини-библиотека с лёгкими функциями

#define RX_PIN 4    // пин
byte data[10];      // приёмный буфер (байты) 
// МИНИМУМ НА 4 БАЙТА БОЛЬШЕ, ЧЕМ ОЖИДАЕМАЯ ДАТА

void setup() {
  Serial.begin(9600);

  // ПИН ОБЯЗАТЕЛЬНО PULLUP!!!111
  pinMode(RX_PIN, INPUT_PULLUP);
}

void loop() {
  // ждём приём
  if (GBUS_read_raw(RX_PIN, data, sizeof(data))) {
    // если успешно приняли, выводим
    for (byte i = 0; i < sizeof(data); i++) {
      Serial.print(data[i]);
      Serial.print(", ");
    }
    Serial.println();
  }
}
