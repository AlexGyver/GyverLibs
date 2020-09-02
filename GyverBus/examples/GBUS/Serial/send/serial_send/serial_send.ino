// отправляем данные через аппаратный UART
// принимает пример serial_send_read
// у этой ардуины провод подключен к пину TX (пин 1)

#define RX_ADDR 3   // адрес приёмника

void setup() {
  Serial.begin(300);  // скорость настраиваем как в GBUS!
}

void loop() {
  Serial.write(4);    // количество байт
  Serial.write(RX_ADDR);  // отправляем на адрес приёмника
  Serial.write(5);    // допустим наш адрес 5
  Serial.write(123);  // дата
  delay(1000);
}
