// При помощи CRGBtoData() можно конвертировать CRGB в mDATA!
// Используем быструю математику FastLED и компактный вывод microLED
// для достижения максимума каефа при минимуме занятой памяти

// ===============================================================
#include <FastLEDsupport.h> // вкл поддержку FL
DEFINE_GRADIENT_PALETTE( heatmap_gp ) {   // делаем палитру огня
  0,     0,  0,  0,     // black
  128,   255,  0,  0,   // red
  224,   255, 255,  0,  // bright yellow
  255,   255, 255, 255  // full white
};
CRGBPalette16 fire_p = heatmap_gp;
// ===============================================================

// микролед
#define LEDPIN 2
#include <microLED.h>
microLED<0, LEDPIN, MLED_NO_CLOCK, LED_WS2818, ORDER_GRB, CLI_AVER> strip;

void setup() {
  strip.setBrightness(150);
}

void loop() {
  static int count = 0;
  count += 10;
  strip.begin();
  for (int i = 0; i < 50; i++)
    strip.send(CRGBtoData(ColorFromPalette(fire_p, inoise8(i * 25, count), 255, LINEARBLEND)));

  strip.end();
  delay(30);
}


/*
  // тинилед
  #define TLED_PORT PORTD  // выводил на Нанку, портД
  #define TLED_DDR DDRD
  #include <tinyLED.h>
  tinyLED<3> strip;
  void setup() {
  strip.setBrightness(100);
  }
  void loop() {
  static int count = 0;
  count += 10;
  for (int i = 0; i < 50; i++)
    strip.send(CRGBtoData(ColorFromPalette(fire_p, inoise8(i * 25, count), 255, LINEARBLEND)));
  delay(30);
  }
*/
