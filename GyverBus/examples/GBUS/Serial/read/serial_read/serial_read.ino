// принимаем пакет и выводим на дисплей, адрес отправителя отдельно
// readBytes - блокирующая функция!
// отправляет serial_read_send
// у этой ардуины провод подключен к пину RX (пин 0)

#include <LiquidCrystal_I2C.h>
LiquidCrystal_I2C lcd(0x27, 16, 2);

void setup() {
  Serial.begin(300);
  Serial.setTimeout(100);
  lcd.init();
  lcd.backlight();
}

byte buf[10];
void loop() {
  if (Serial.readBytes(buf, 6)) {
    lcd.clear();
    lcd.home();
    lcd.print("tx: ");
    lcd.print(buf[2]);
    lcd.setCursor(0, 1);
    int val = buf[3] | (buf[4] << 8);
    lcd.print(val);
  }
}
