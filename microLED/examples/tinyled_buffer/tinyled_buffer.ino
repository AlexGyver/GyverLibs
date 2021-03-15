// пример с tinyLED, должно работать на всех тиньках и мегах
// пример с программным буфером сделаем для эффектов движения

// дефайны настроек, прописываются перед подключением либы
#define TLED_ORDER ORDER_GRB   // порядок цветов
#define TLED_CHIP LED_WS2812   // чип светодиода ленты

// либа
#define COLOR_DEBTH 2 // чтобы память сэкономить, таки буфер
#include "tinyLED.h"
tinyLED<3> strip;   // указываем пин (в порядке порта)

#define NUMLEDS 20 	// количество светодиодов
mData buf[NUMLEDS];	// буфер

void setup() {
  // strip.setBrightness(120);  // яркость
}

void loop() {
  // через вызов включаем крайний лед чёрным или случайно цветным
  static bool flag = 0;
  flag = !flag;
  buf[NUMLEDS - 1] = mWheel8(random(255)) * flag;

  // перемотка буфера на 1
  for (int i = 0; i < NUMLEDS; i++) buf[i] = buf[i + 1];

  // и вот тут - выводим буфер на ленту
  strip.sendBuffer(buf, NUMLEDS);
  delay(150);
}
