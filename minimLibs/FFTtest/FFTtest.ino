// преобразование Фурье на C (для есп8266)

#define FFT_SIZE 64  // 32/64/128/256/512
#include "FFT_C.h"
#include "VolAnalyzer.h"
VolAnalyzer low, high;

// это всё нинада, для тестов
int raw0[] = {24, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 20, 51, 86, 114, 151, 183, 219, 246, 292, 325, 352, 369, 398, 396, 399, 394, 392, 373, 369, 368, 363, 346, 333, 300, 277, 243, 214, 170, 125, 89, 44, 14, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 21, 67, 97, 135, 181, 210, 243, 279, 310, 332, 362, 376, 381, 388, 393, 394, 394, 399, 403, 392, 379, 367, 344, 306, 265, 226, 182, 150, 118, 79, 44, 31, 2,};
int raw1[] = {0, 0, 138, 189, 68, 0, 0, 0, 73, 189, 139, 0, 0, 0, 13, 163, 181, 48, 0, 0, 0, 99, 194, 108, 0, 0, 0, 21, 171, 171, 17, 0, 0, 0, 123, 193, 102, 0, 0, 0, 50, 178, 164, 8, 0, 0, 0, 140, 190, 66, 0, 0, 0, 75, 193, 144, 0, 0, 0, 4, 166, 189, 58, 0, 0, 0, 105, 205, 117, 0, 0, 0, 34, 187, 174, 22, 0, 0, 0, 128, 196, 91, 0, 0, 0, 40, 176, 152, 2, 0, 0, 0, 129, 177, 66, 0, 0, 0, 57, 183, 135, 0, 0, 0, 0, 140, 176, 45, 0, 0, 0, 79, 186, 109, 0, 0, 0, 4, 154, 168, 21, 0, 0, 0, 100, 185, 102, 0,};
int raw2[] = {0, 85, 0, 156, 108, 0, 175, 81, 0, 170, 60, 0, 142, 69, 0, 165, 69, 0, 163, 68, 0, 165, 66, 0, 136, 55, 0, 182, 65, 0, 184, 75, 0, 142, 69, 0, 175, 74, 0, 202, 82, 0, 173, 72, 0, 174, 67, 0, 184, 68, 0, 197, 56, 0, 183, 68, 0, 188, 61, 0, 201, 51, 0, 211, 46, 0, 176, 46, 0, 182, 52, 0, 180, 43, 0, 188, 42, 0, 186, 62, 0, 194, 32, 0, 181, 34, 0, 190, 45, 0, 165, 44, 0, 177, 70, 0, 168, 57, 0, 186, 69, 0, 199, 26, 0, 183, 39, 0, 174, 45, 0, 182, 30, 0, 187, 32, 0, 188, 61, 0, 182, 61, 0, 182, 33, 0, 194, 40,};
// это всё нинада

int raw[FFT_SIZE];
int spectr[FFT_SIZE];

#define STRIP_PIN D4        // пин ленты
#define MAX_LEDS 512        // макс. светодиодов
#define STRIP_CHIP WS2812   // чип ленты
#define STRIP_COLOR GRB     // порядок цветов в ленте
#include <FastLED.h>        // лента
CRGB leds[8];

void setup() {
  FastLED.addLeds<STRIP_CHIP, STRIP_PIN, STRIP_COLOR>(leds, 8);
  FastLED.setBrightness(100);
  Serial.begin(9600);

  low.setDt(0);
  low.setPeriod(0);
  low.setWindow(0);
  high.setDt(0);
  high.setPeriod(0);
  high.setWindow(0);

  low.setVolK(25);
  high.setVolK(25);
  low.setTrsh(50);
  high.setTrsh(50);
  low.setVolMax(150);
  high.setVolMax(150);
  low.setVolMin(30);
  high.setVolMin(30);

  /* 
    // рабочие моменты с FHT
    delay(3000);
    uint32_t us = micros();
    FFT(raw0, spectr);
    us = micros() - us;
    Serial.println(us);
    //for (int i = 0; i < 3; i++) {
    FFT(raw0, spectr);
    for (int k = 0; k < FFT_SIZE / 2; k++) {
    Serial.print(int(spectr[k]));
    Serial.print(' ');
    }
    Serial.println();
    FFT(raw1, spectr);
    for (int k = 0; k < FFT_SIZE / 2; k++) {
    Serial.print(int(spectr[k]));
    Serial.print(' ');
    }
    Serial.println();
    FFT(raw2, spectr);
    for (int k = 0; k < FFT_SIZE / 2; k++) {
    Serial.print(int(spectr[k]));
    Serial.print(' ');
    }
    Serial.println();
    //}
  */

}

void loop() {
  for (int i = 0; i < FFT_SIZE; i++) raw[i] = analogRead(A0);

  FFT(raw, spectr);
  int low_raw = 0;
  int high_raw = 0;
  for (int i = 0; i < FFT_SIZE / 2; i++) {
    spectr[i] = (spectr[i] * (i + 2)) >> 1;
    if (i < 2) low_raw += spectr[i];
    else high_raw += spectr[i];
  }

  low.tick(low_raw);
  high.tick(high_raw);

  FastLED.clear();
  //fill_solid(leds, low.getVol() * 8 / 150, CHSV(0, 255, 255));    // полоса громкости
  fill_solid(leds, 8, CHSV(0, 255, high.getVol()));
  fill_solid(leds, 4, CHSV(150, 255, low.getVol()));
  FastLED.show();
  Serial.print(low.getVol());
  Serial.print(' ');
  Serial.println(high.getVol());

  /*
    for (int i = 0; i < FFT_SIZE / 2; i++) {
    if (spectr[i] < 10) spectr[i] = 0;
    Serial.print(int(spectr[i]));
    Serial.print(' ');
    }
    Serial.println();
  */
}
