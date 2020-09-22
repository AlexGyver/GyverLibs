// принимаем запрос и отправляем в ответ данные с АЦП

#define RX_PIN 4      // пин
#define RX_ADDR 3     // наш адрес
#include "GBUSmini.h"	// мини-библиотека с лёгкими функциями

void setup() {
  Serial.begin(9600);
}

void loop() {
  // ждём приём на адрес RX_ADDR, т.е. наш адрес RX_ADDR
  byte txaddr = GBUS_read_request(RX_PIN, RX_ADDR);
  if (txaddr) {
    // если успешно приняли
    // например читаем АЦП
    int val = analogRead(0);

    Serial.print("sending ");
    Serial.println(val);

    // разбиваем на байты
    byte b1 = lowByte(val);
    byte b2 = highByte(val);
    byte buf[] = {b1, b2};
    
    // отправляем обратно (по txaddr)
    // с нашего адреса (RX_ADDR)
    // c контролем целостности данных
    GBUS_send(RX_PIN, txaddr, RX_ADDR, buf, sizeof(buf));
  }
}
