#include <GyverOLED.h>
GyverOLED<SSD1306_128x64, OLED_NO_BUFFER> oled;
// можно передать адрес: GyverOLED oled(0x3C);

void setup() {
  oled.init();              // инициализация
  oled.clear();
}

void loop() {
  for (byte i = 0; i < 100; i += 5) {
    oled.setCursorXY(0, 0);
    drawBattery(i);
    delay(50);
  }
  for (int i = 100; i > 0; i -= 5) {
    oled.setCursorXY(0, 0);
    drawBattery(i);
    delay(50);
  }
}

void drawBattery(byte percent) {
  oled.drawByte(0b00111100);  // пипка
  oled.drawByte(0b00111100);
  oled.drawByte(0b11111111);  // стенка
  for (byte i = 0; i < (100 - percent) / 8; i++) oled.drawByte(0b10000001);
  for (byte i = 0; i < (percent) / 8; i++) oled.drawByte(0b11111111);
  oled.drawByte(0b11111111);  // попка
}
