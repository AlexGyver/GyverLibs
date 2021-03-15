// пример с tinyLED, вывод через АППАРАТНЫЙ SPI (работает либа SPI.h или другая)
// где у твоего камня SPI - смотри распиновку!
// пример проверен на Ardu Nano, Data D11, Clock D13

// дефайны настроек, прописываются перед подключением либы
#define TLED_ORDER ORDER_BGR       // порядок цветов
#define TLED_CHIP LED_APA102_SPI   // чип светодиода ленты
//#define TLED_SPI_CLOCK 8000000	// скорость SPI

// либа
#include "tinyLED.h"
tinyLED strip; // для LED_APA102_SPI не указываем пины!

#define NUMLEDS 50 // количество светодиодов (для циклов)

void setup() {
  strip.setBrightness(120);  // яркость

  // каждый вывод "задвигает" указанный цвет
  // =================================================
  strip.clear(NUMLEDS);
  strip.begin();
  strip.sendRGB(255, 255, 255); // отправить r,g,b на первый лед
  strip.send(mOrange);      // отправить оранжевый
  strip.send(mAqua);        // голубой
  strip.send(mPurple);      // и розовый
  strip.end();
  // леды загорятся вот так:
  // начало--розовый--аква--оранж--белый
  delay(5000);              // пауза

  // =================================================
  // зальём 50 ледов белым. Вывод начнётся с начала!!
  strip.begin();
  for (byte i = 0; i < 10; i++) strip.send(mYellow);
  strip.end();
  delay(5000);              // пауза

  // =================================================
  // зальём 50 ледов с чередованием жёлтый-чёрный
  strip.begin();
  for (byte i = 0; i < 50; i++) {
    strip.send(i % 2 == 0 ? mYellow : mBlack);
  }
  strip.end();
  delay(5000);              // пауза
}

void loop() {
  // =================================================
  // иииии бегущая радуга!
  static byte count;
  count++;
  strip.begin();
  for (int i = 0; i < NUMLEDS; i++) {
    strip.send(mWheel8(count + i * 255 / NUMLEDS)); // выводим радугу
  }
  strip.end();
  delay(30);
}
