// пример работы с лентой без буфера

#define STRIP_PIN 2     // пин ленты
#define NUMLEDS 20      // кол-во светодиодов

#include <microLED.h>
// количество светодиодов указываем 0
microLED<0, STRIP_PIN, LED_WS2818, ORDER_GRB, CLI_AVER> strip;

void setup() {
  strip.setBrightness(150);

  // алгоритм вывода такой:
  // strip.begin();
  // strip.send(цвет);
  // strip.send(цвет);
  // ..........
  // strip.end();

  // функция send(цвет) загружает следующий цвет в ленту
  // при каждом новом вызове цвет передаётся "паравозиком" по ленте,
  // таким образом можно выводить на ленту любой длины.
  // Между вызовами send() должно пройти МЕНЬШЕ 50 мкс (для 2812), иначе вывод будет прерван
  // Это усложняет генерацию эффектов "на лету", потому что для некоторых банально не хватит скорости вычисления

  strip.begin();
  for (int i = 0; i < NUMLEDS; i++) {
    strip.send(mWheel8(i * 255 / NUMLEDS)); // выводим радугу
  }
  strip.end();
  delay(2000);
}

void loop() {
  // а тут давайте запустим генерацию гармонического шума по палитре радуги с дополнительным смещением спектра
  static int counter = 0;
  static byte counter2 = 0;
  counter += 10;
  counter2++;
  strip.begin();
  for (int i = 0; i < NUMLEDS; i++) strip.send(mWheel8(counter2 + inoise8(i * 50, counter)));
  strip.end();
  delay(30);
  // скорости достаточно! Эффект работает
}
