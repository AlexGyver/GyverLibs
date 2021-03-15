// При помощи CRGBtoData() можно конвертировать CRGB в mDATA!
// Используем быструю математику FastLED и компактный вывод microLED
// для достижения максимума каефа при минимуме занятой памяти
#define LEDPIN 2
#include <microLED.h>
#include <FastLEDsupport.h> // вкл поддержку
microLED<0, LEDPIN, MLED_NO_CLOCK, LED_WS2818, ORDER_GRB, CLI_AVER> strip;

void setup() {
  strip.setBrightness(150);
}

void loop() {
  static byte count = 0;
  static int count2 = 0;
  count++;
  count2 += 10;
  strip.begin();
  for (int i = 0; i < 50; i++) {
    // движется палитра полосатая
    //strip.send(CRGBtoData(ColorFromPalette(RainbowStripeColors_p, count + i * 3, 255, LINEARBLEND)));

    // рандомный шум радугой
    strip.send(CRGBtoData(ColorFromPalette(RainbowColors_p, inoise8(i * 20, count2), 255, LINEARBLEND)));
  }
  strip.end();
  delay(40);
}

/*
  // =========== ТО ЖЕ САМОЕ ДЛЯ ТИНИЛЕД ==========
  #define TLED_PORT PORTD  // выводил на Нанку, портД (пин D3)
  #define TLED_DDR DDRD
  #include <tinyLED.h>
  #include <FastLEDsupport.h>
  tinyLED<3> strip;
  void setup() {
  strip.setBrightness(100);
  }
  void loop() {
  static byte count = 0;
  static int count2 = 0;
  count++;
  count2 += 10;
  for (int i = 0; i < 50; i++) {
    // движется палитра полосатая
    //strip.send(CRGBtoData(ColorFromPalette(RainbowStripeColors_p, count + i * 3, 255, LINEARBLEND)));

    // рандомный шум радугой
    strip.send(CRGBtoData(ColorFromPalette(RainbowColors_p, inoise8(i * 20, count2), 255, LINEARBLEND)));
  }
  delay(40);
  }
*/
