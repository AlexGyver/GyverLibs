/*
   Простой пример вывода целых чисел (unsigned int / int), С НУЛЯМИ СЛЕВА, на дисплей TM74HC595
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
  int value = random(-99, 99);    // Получим случайное число со знаком в диапазоне (-99...99)
  disp.displayIntNulls(value);    // Вывод числа С НУЛЯМИ СЛЕВА (минус всегда крайний слева)
  delay(500);                     // Задержка между сменой
}

ISR(TIMER2_A) {        // Прерывание Timer 2
  disp.tickManual();   // Обслуживание динамической индикации дисплея
}
