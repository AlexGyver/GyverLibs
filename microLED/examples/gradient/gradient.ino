// пример работы с градиентами

#define STRIP_PIN 2     // пин ленты
#define NUMLEDS 20      // кол-во светодиодов

#include <microLED.h>
microLED<NUMLEDS, STRIP_PIN, LED_WS2818, ORDER_GRB, CLI_AVER> strip;

void setup() {
  strip.setBrightness(100);

  // в библиотеке есть два инструмента градиентной заливки:
  // простой двойной и на любое количество точек

  // ===== ДВОЙНОЙ ГРАДИЕНТ =====
  // заливает (от, до, цвет1, цвет2)
  strip.fillGradient(0, 10, mRed, mWhite);
  strip.show();
  delay(2000);

  // можно указать любой цвет в формате mData
  strip.fillGradient(0, 10, mWheel8(30), mRGB(0, 120, 90));
  strip.show();
  delay(2000);

  // ===== СЛОЖНЫЙ ГРАДИЕНТ =====
  mGradient<4> myGrad;  // создать градиент с именем myGrad на 4 точки
  // указываем цвет каждой точки
  myGrad.colors[0] = mBlack;
  myGrad.colors[1] = mRed;
  myGrad.colors[2] = mYellow;
  myGrad.colors[3] = mWhite;
  // создали градиент чёрный-красный-жёлтый-белый с одинаковым расстоянем между точками

  // при помощи .get(позиция, размер) можно получить цвет в заданном месте градиента при указанной длине

  // заливаем всю ленту
  for (int i = 0; i < NUMLEDS; i++) {
    strip.set(i, myGrad.get(i, NUMLEDS)); // получаем последовательно все цвета
  }
  strip.show();
  delay(2000);

  // можно залить половину ленты всем градиентом и от половины развернуть в другую сторону
  for (int i = 0; i < NUMLEDS / 2; i++) strip.set(i, myGrad.get(i, NUMLEDS / 2));
  for (int i = 0; i < NUMLEDS / 2; i++) strip.set(NUMLEDS / 2 + i, myGrad.get(NUMLEDS / 2 - i, NUMLEDS / 2));
  strip.show();

  // продолжение смотри в примере с шумом
}

void loop() {
}
