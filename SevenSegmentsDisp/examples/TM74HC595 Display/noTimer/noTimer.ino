/*
   Простой пример вывода данных на дисплей TM74HC595, БЕЗ использования прерываний таймера.
   В этом случае вывод бегущей строки и вообще любые другие блокирующие функции в цикле loop
   приведут к потере работоспособности дисплея.
   Необходимо отказаться от delay и использовать таймеры на millis() / micros()
   https://alexgyver.ru/lessons/time/
*/

#define DATA_PIN   11
#define SCLK_PIN   10
#define RCLK_PIN   12

/* Периоды встроенного в .tick() таймера можно указать вручную ПЕРЕД подключенеим библиотеки */
// #define DISP595_FAST_PERIOD 300  // Период при использовании регулировки яркости (по умолч. 500 мкс)
// #define DISP595_SLOW_PERIOD 1000 // период при использовании статической яркости (по умолч. 3500 мкс)

#include <SevenSegmentsDisp.h>

Disp595 disp(DATA_PIN, SCLK_PIN, RCLK_PIN);

void setup() {}

void loop() {
  /* Использование встроенного таймера на micros */
  disp.tick(); // Таймер уже встроен в .tick()

  /* Все остальные задачи не блокируют цикл loop */
  static uint32_t timer = millis();
  if (millis() - timer >= 500) {
    timer = millis();                // Каждые 500 мс
    disp.displayInt(analogRead(A0)); // Выводим актуальное значение с аналог. пина A0
  }

  /* Ручное обслуживание дисплея при помощи таймера на micros */
  /*
    static uint32_t dispTimer = micros();
    if (micros() - dispTimer >= DISP_PERIOD) {
    dispTimer = micros();
    disp.tickManual(); // "голый" .tick()
    }
  */
}
