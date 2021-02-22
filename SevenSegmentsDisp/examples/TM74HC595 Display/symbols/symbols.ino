/*
   Простой пример прямого вывода данных на дисплей TM74HC595
   Обслуживание динамической индикации берет на себя аппаратный Timer 2
   https://alexgyver.ru/lessons/timer-isr/
*/

#define DATA_PIN   11
#define SCLK_PIN   10
#define RCLK_PIN   12
#define DISP_PERIOD 3000  // Период динамической индикации в мкс (500-6000)

#include <GyverTimers.h>
#include <SevenSegmentsDisp.h>

Disp595 disp(DATA_PIN, SCLK_PIN, RCLK_PIN);

void setup() {
  Timer2.setPeriod(DISP_PERIOD);  // Заводим Timer 2 на прерывания с нужным периодом
  Timer2.enableISR();             // Включаем прерывания Timer 2
}

void loop() {
  disp.displayBytes(_H, _E, _L, _P);            // Вывод сразу 4х символов
  delay(1000);                                  // Задержка между сменой
  disp.displayBytes(_U, _S, _empty, _empty);    // Доступны не все символы! (см. GyverTM74HC595.h)
  delay(1000);                                  // Задержка между сменой

  disp.clear();                                 // Очистка дисплея

  disp.displayByte(_dash, 3);                   // Вывод байта (символа) на нужную позцию (0 - 3) слева
  disp.displayByte(_under, 2);
  disp.displayByte(_dash, 1);
  disp.displayByte(_under, 0);
  delay(1000);

  uint8_t str[] = {_P, _H} ;                          // Выводим массив из нескольких байт (остальные символы остаются нетронутыми)
  disp.clear();                                       // Очистка дисплея
  disp.displayBytes(str, sizeof(str));                // Вывод символов с выравниванием по ЛЕВОМУ краю
  delay(1000);

  disp.clear();
  disp.displayBytes(str, sizeof(str), ALIGN_RIGHT);   // Выраванивание можно сделать и по правому краю
  delay(1000);

  disp.clear();
  disp.displayBytes(str, sizeof(str), ALIGN_LEFT);    // Или же вручную укзать левый край
  delay(1000);
}

ISR(TIMER2_A) {        // Прерывание Timer 2
  disp.tickManual();   // Обслуживание динамической индикации дисплея
}
