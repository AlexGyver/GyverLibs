#include <GyverOLED.h>
//GyverOLED<SSD1306_128x32, OLED_BUFFER> oled;
//GyverOLED<SSD1306_128x32, OLED_NO_BUFFER> oled;
//GyverOLED<SSD1306_128x64, OLED_BUFFER> oled;
//GyverOLED<SSD1306_128x64, OLED_NO_BUFFER> oled;
//GyverOLED<SSD1306_128x64, OLED_BUFFER, OLED_SPI, 8, 7, 6> oled;
GyverOLED<SSH1106_128x64> oled;
// можно передать адрес: GyverOLED oled(0x3C);

void setup() {
  oled.init();              // инициализация
  oled.clear();
}

void loop() {
  for (byte i = 0; i < 100; i += 5) {
    oled.clear();
    oled.setCursorXY(0, 0);
    drawBattery(i);
    oled.update();
    delay(50);
  }
  for (int i = 100; i > 0; i -= 5) {
    oled.clear();
    oled.setCursorXY(0, 0);
    drawBattery(i);
    oled.update();
    delay(50);
  }
}

void drawBattery(byte percent) {
  oled.drawByte(0b00111100);  // пипка
  oled.drawByte(0b00111100);
  oled.drawByte(0b11111111);  // стенка
  for (byte i = 0; i < 100 / 8; i++) {
    if (i < (100 - percent) / 8) oled.drawByte(0b10000001);
    else oled.drawByte(0b11111111);
  }
  oled.drawByte(0b11111111);  // попка
}
