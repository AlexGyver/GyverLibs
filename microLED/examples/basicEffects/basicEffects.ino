// простейшие динамические эффекты
// сначала ознакомься с примером microLED_guide !!!

#define STRIP_PIN 2     // пин ленты
#define NUMLEDS 20      // кол-во светодиодов

#define COLOR_DEBTH 3
#include <microLED.h>   // подключаем библу
microLED<NUMLEDS, STRIP_PIN, MLED_NO_CLOCK, LED_WS2818, ORDER_GRB, CLI_AVER> strip;

void setup() {
  strip.setBrightness(60);
}

void loop() {
  // раскомментируй нужный эффект
  
  rainbow();      // бегущая радуга во всю ленту
  //filler();       // плавное заполнение
  //colorCycle();   // плавная смена цвета
  //runningDots();  // бегущие точки

  // вывод
  breathing();    // "дыхание" яркости, применяется ко всем эффектам
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

void filler() {
  static int counter = 0;
  strip.clear();
  strip.fill(0, counter, mRed);
  counter++;
  if (counter >= NUMLEDS) counter = 0;
}

void colorCycle() {
  static byte counter = 0;
  strip.fill(mWheel8(counter));
  counter += 3;
}

void runningDots() {
  static byte counter = 0;
  // перемотка буфера со сдвигом (иллюзия движения пикселей)
  for (int i = 0; i < NUMLEDS - 1; i++) strip.leds[i] = strip.leds[i + 1];

  // каждый третий вызов - последний пиксель красным, иначе чёрным
  if (counter % 3 == 0) strip.leds[NUMLEDS - 1] = mRed;
  else strip.leds[NUMLEDS - 1] = mBlack;
  counter++;
  delay(100);   // дополнительная задержка
}

void breathing() {
  static int dir = 1;
  static int bright = 0;
  bright += dir * 5;    // 5 - множитель скорости изменения

  if (bright > 255) {
    bright = 255;
    dir = -1;
  }
  if (bright < 0) {
    bright = 0;
    dir = 1;
  }
  strip.setBrightness(bright);
}
