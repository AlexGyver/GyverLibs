// пример работы с несколькими лентами
#define LED1_PIN 6      // пин ленты 1
#define LED2_PIN 7      // пин ленты 1
#define NUMLEDS_1 60    // кол-во светодиодов ленты 1
#define NUMLEDS_2 120   // кол-во светодиодов ленты 2

// настройки влияют на обе ленты сразу!
#define COLOR_DEBTH 2   // цветовая глубина: 1, 2, 3 (в байтах)
#include <microLED.h>

microLED<NUMLEDS_1, LED1_PIN, MLED_NO_CLOCK, LED_WS2818, ORDER_GRB, CLI_AVER> strip_1;
microLED<NUMLEDS_2, LED2_PIN, MLED_NO_CLOCK, LED_WS2818, ORDER_GRB, CLI_AVER> strip_2;

void setup() {
  // управляем яркостью
  strip_1.setBrightness(30);
  strip_2.setBrightness(100);

  // можно залить цветом
  strip_1.fill(mYellow);
  strip_2.fill(mHSV(50, 255, 255));

  // нулевой светодиод второй ленты сделаем красным
  strip_2.leds[0] = mRGB(255, 0, 0);

  // выводим на ленты
  strip_1.show();
  strip_2.show();
}

void loop() {
}
