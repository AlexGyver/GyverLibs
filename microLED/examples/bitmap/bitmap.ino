#define STRIP_PIN 2     // пин ленты

// для генерации битмапа используется программа imageProcessor
// https://github.com/AlexGyver/imageProcessor
// со стандартными настройками, выбираем только глубину цвета (RGB888/565/232)
// остальные - PROGMEM вкл, массив 1D
const uint16_t bitmap_8x8[] PROGMEM = {
  0x0000, 0x0000, 0x0000, 0xF809, 0xF809, 0xF809, 0xFF9D, 0x0000, 
  0x0000, 0x0000, 0x0000, 0xF809, 0xF809, 0xF809, 0xF809, 0xF809, 
  0x0000, 0x0000, 0xAA86, 0xFE75, 0xAA86, 0x0000, 0xFE75, 0x0000, 
  0x0000, 0x0000, 0xAA86, 0xFE75, 0xFE75, 0xAA86, 0xAA86, 0xFE75, 
  0x0000, 0x0000, 0x0000, 0xAA86, 0xFE75, 0xFE75, 0xFE75, 0x0000, 
  0x0000, 0xF809, 0xF809, 0xFFE4, 0x2D7F, 0x2D7F, 0xFD00, 0x0000, 
  0xFF9D, 0x0000, 0x2D7F, 0x2D7F, 0x2D7F, 0x2D7F, 0x194A, 0x83B3, 
  0x0000, 0x0000, 0xAA86, 0x0000, 0x0000, 0x0000, 0x792A, 0x0000, 
};
#define COLOR_DEBTH 2
#include <microLED.h>
microLED<32 * 8, STRIP_PIN, MLED_NO_CLOCK, LED_WS2812, ORDER_GRB, CLI_HIGH> matrix(32, 8, ZIGZAG, LEFT_TOP, DIR_DOWN);

void setup() {
  matrix.setBrightness(150);
  for (int x = -10; x < 10; x++) {
    matrix.drawBitmap16(x, x, bitmap_8x8, 8, 8);
    matrix.show();
    delay(500);
    matrix.clear();
  }
  // также есть drawBitmap8 и drawBitmap32
  // (x, y, массив, ширина, высота)
}

void loop() {
}
