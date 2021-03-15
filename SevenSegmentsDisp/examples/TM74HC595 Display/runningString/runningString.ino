/*
   Простой пример вывода бегущей строки на дисплей TM74HC595
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
  uint8_t str[] = {                                     // Создаем массив с нашей строкой
    _t, _E, _S, _t, _empty, _dash, _1, _2, _3, _4, _5   // Доступны не все символы!
  };
  
  // Так как используется таймер, встроенный в функцию вывода tick использовать НЕ НУЖНО, просто не указываем 4й аргумент или указываем его как false
  disp.runningString(str, sizeof(str), 150);            // Передаем строку, ее размер, задержку между сдвигами в мс
  //disp.runningString(str, sizeof(str), 150, false);   // Можно явно отключить встроенный tick, 4м аргументом
}

ISR(TIMER2_A) {        // Прерывание Timer 2
  disp.tickManual();   // Обслуживание динамической индикации дисплея
}
