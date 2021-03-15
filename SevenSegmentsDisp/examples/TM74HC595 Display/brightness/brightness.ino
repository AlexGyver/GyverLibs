/*
   Простой пример программного управления яркостью
   Обслуживание динамической индикации берет на себя аппаратный Timer 2
   https://alexgyver.ru/lessons/timer-isr/
*/

#define DATA_PIN   11
#define SCLK_PIN   10
#define RCLK_PIN   12
#define DISP_PERIOD 500             // Период динамической индикации в мкс, делаем почаще!
#define DISP595_BRIGHTNESS          // Сообщаем библиотеке, что будем использовать регулировку яркости! 
// #define DISP595_BRIGHTNESS_DEPTH 10   // Глубину яркости можно настроить, больше - плавнее, но сильнее мерцание (по умолч. 8)

#include <GyverTimers.h>
#include <SevenSegmentsDisp.h>

Disp595 disp(DATA_PIN, SCLK_PIN, RCLK_PIN);

void setup() {
  Timer2.setPeriod(DISP_PERIOD);                // Заводим Timer 2 на прерывания с нужным периодом
  Timer2.enableISR();                           // Включаем прерывания Timer 2

  disp.displayBytes(_t, _E, _S, _t);            // Вывод символов
}

void loop() {
  disp.brightness(0);                           // Устанавливаем яркость в диапазоне (0 - 7)
  delay(1000);
  disp.brightness(3);
  delay(1000);
  disp.brightness(7);
  delay(1000);
}

ISR(TIMER2_A) {  // Прерывание Timer 2
  disp.tickManual();   // Обслуживание динамической индикации дисплея
}
