// более интересный пример
// принимаем данные из примера serial_send_read_3
// у этой ардуины провод подключен к пину RX_PIN
#include "GyverBus.h"

#define RX_PIN 4    // пин
#define RX_ADDR 3   // наш адрес

// пин 4, адрес 3, приёмник на 15 байт
GBus<RX_PIN, GBUS_RX, GBUS_NO_CRC> rx(RX_ADDR, 15);

// та же структура, но БЕЗ ПЕРВЫХ ТРЁХ БАЙТОВ!!!
struct myStruct {
  byte val_b;
  int val_i;
  float val_f;
};
myStruct rx_data;

void setup() {
  Serial.begin(9600);
}

void loop() {
  rx.tick();

  if (rx.gotData()) {
    // читаем (служебные байты автоматически отбросятся)
    rx.readData(rx_data);

    // выводим дату
    Serial.println(rx_data.val_b);
    Serial.println(rx_data.val_i);
    Serial.println(rx_data.val_f);
    Serial.print("from address: ");
    Serial.println(rx.getTXaddress());
    Serial.println();
  }
}
