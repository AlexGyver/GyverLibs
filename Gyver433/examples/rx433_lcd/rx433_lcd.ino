// крупный приёмник 5.0V SYN480R
#define G433_BUFSIZE 50   // размер буфера
#define G433_SPEED 2000   // скорость бит/сек (минимальная)

#include <Gyver433.h>
Gyver433_RX rx(2);  // указали пин

#include <Wire.h>
#include <LiquidCrystal_I2C.h>
LiquidCrystal_I2C lcd(0x3f, 16, 2); // или 0x27

void setup() {
  Serial.begin(9600);
  lcd.init();
  lcd.backlight();
}

void loop() {
  // tick принимает асинхронно, но может ловить ошибки при загруженном коде
  // tickWait блокирует выполнение, но принимает данные чётко
  if (rx.tick()) {
    byte buf[64];
    rx.readData(buf); // прочитать в buf
    lcd.clear();
    lcd.home();    
    for (byte i = 0; i < rx.size; i++) lcd.write(buf[i]);
  }
}
