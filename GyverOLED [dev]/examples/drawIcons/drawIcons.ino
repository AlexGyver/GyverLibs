#include <GyverOLED.h>

GyverOLED<SSD1306_128x64, OLED_NO_BUFFER> oled;

const static uint8_t icons_7x7[][7] PROGMEM = {
  {0x7f, 0x45, 0x49, 0x51, 0x49, 0x45, 0x7f}, //0 email
  {0x40, 0x60, 0x77, 0x77, 0x77, 0x60, 0x40}, //1 user
  {0x70, 0x7e, 0x71, 0x51, 0x71, 0x7e, 0x70}, //2 lock, password
};

const static uint8_t icons_8x8[][8] PROGMEM = {
  {0x7e, 0x81, 0x81, 0x91, 0x88, 0x85, 0x83, 0x77}, //0 maximize
  {0x7c, 0x44, 0xe7, 0xa5, 0xa5, 0xe7, 0x44, 0x7c}, //1 printer
  {0x7e, 0x81, 0x91, 0xa1, 0x91, 0x89, 0x84, 0x72}, //2 checked
  {0x7e, 0x81, 0x81, 0x81, 0x81, 0x81, 0x81, 0x7e}, //3 unchecked
  {0x1c, 0x3e, 0x73, 0xe1, 0xe1, 0x73, 0x3e, 0x1c}, //4 map
};

void setup() {

  oled.init();              // инициализация

  // ускорим вывод, ВЫЗЫВАТЬ ПОСЛЕ oled.init()!!!
  Wire.setClock(400000L);   // макс. 800'000

  oled.clear();
  oled.home();

  drawIcon7x7(0);
  oled.setCursor(1 << 3, 0);
  drawIcon7x7(1);
  oled.setCursor(2 << 3, 0);
  drawIcon7x7(2);

  oled.setCursor(0 << 3, 2);
  drawIcon8x8(0);
  oled.setCursor(2 << 3, 2);
  drawIcon8x8(1);
  oled.setCursor(4 << 3, 2);
  drawIcon8x8(2);
  oled.setCursor(6 << 3, 2);
  drawIcon8x8(3);
  oled.setCursor(8 << 3, 2);
  drawIcon8x8(4);
}

void loop() {
}


void drawIcon7x7(byte index) {
  size_t s = sizeof icons_7x7[index];//можно так, а можно просто 7 
  for(unsigned int i = 0; i < s; i++) {
    oled.drawByte(pgm_read_byte(&(icons_7x7[index][i])));
  }
}

void drawIcon8x8(byte index) {
  size_t s = sizeof icons_8x8[index];//можна так, а можна просто 8 
  for(unsigned int i = 0; i < s; i++) {
    oled.drawByte(pgm_read_byte(&(icons_8x8[index][i])));
  }
}
