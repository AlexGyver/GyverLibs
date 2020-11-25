// пример работы с LED_APA102 программно
#define DATA_PIN 11
#define CLOCK_PIN 13
#define NUMLEDS 50      // кол-во светодиодов

#include <microLED.h>   // подключаем библу
microLED<NUMLEDS, DATA_PIN, CLOCK_PIN, LED_APA102, ORDER_BGR, CLI_AVER> strip;

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
