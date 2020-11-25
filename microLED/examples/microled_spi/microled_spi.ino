// пример работы с LED_APA102 аппаратно
// где у твоего камня SPI - смотри распиновку!
// пример проверен на Nano, Data D11, Clock D13

#define NUMLEDS 30      // кол-во светодиодов

#define MLED_USE_SPI    // деф до библиотеки для поддержки SPI.h
//#define MLED_SPI_CLOCK 8000000 // можно частоту менять из скетча
#include <microLED.h>   // подключаем библу
microLED<NUMLEDS, MLED_NO_CLOCK, MLED_NO_CLOCK, LED_APA102_SPI, ORDER_BGR, CLI_AVER> strip;

void setup() {
  strip.setBrightness(60);
}

void loop() {
  rainbow();      // бегущая радуга во всю ленту
  strip.show();   // вывод
  delay(30);      // 30 кадров в секунду
}

void rainbow() {
  static byte counter = 0;
  for (int i = 0; i < NUMLEDS; i++) {
    strip.set(i, mWheel8(counter + i * 255 / NUMLEDS));   // counter смещает цвет
  }
  counter += 3;   // counter имеет тип byte и при достижении 255 сбросится в 0
}
