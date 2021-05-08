#include <GyverOLED.h>
//GyverOLED<SSD1306_128x32, OLED_BUFFER> oled;
//GyverOLED<SSD1306_128x32, OLED_NO_BUFFER> oled;
//GyverOLED<SSD1306_128x64, OLED_BUFFER> oled;
//GyverOLED<SSD1306_128x64, OLED_NO_BUFFER> oled;
//GyverOLED<SSD1306_128x64, OLED_BUFFER, OLED_SPI, 8, 7, 6> oled;
GyverOLED<SSH1106_128x64> oled;
// можно передать адрес: GyverOLED oled(0x3C);

void setup() {
  Serial.begin(9600);
  oled.init();              // инициализация
  Wire.setClock(400000L);
  oled.clear();
  oled.update();

  byte textPos1 = 8;
  byte textPos2 = 32;

  oled.createBuffer(5, 0, 66, textPos2 + 8 + 2);

  oled.roundRect(5, textPos1 - 4, 65, textPos1 + 8 + 2, OLED_STROKE);
  oled.setCursorXY(10, textPos1);
  oled.print("SET MODE");

  oled.roundRect(5, textPos2 - 4, 65, textPos2 + 8 + 2, OLED_FILL);
  oled.setCursorXY(10, textPos2);
  oled.invertText(true);
  oled.print("LOL KEK");

  oled.sendBuffer();
  oled.update();  
}
