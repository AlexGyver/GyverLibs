/*
   Простой пример вывода чисел с плавающей точкой (float),БЕЗ НУЛЕЙ СЛЕВА, на дисплей TM74HC595
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
  disp.displayFloat(16.384);          // Вывод числа БЕЗ НУЛЕЙ СЛЕВА (минус следует за ближайшим числом)
  delay(500);                         
  disp.displayFloat(-8.192, 1);       // Можно указать кол-во дробных знаков, (без указания - 1 знак)
  delay(500);                         
  disp.displayFloat(-4.096, 2);       // Можно выводить и отрицательные числа
  delay(500);                         
  disp.displayFloat(2.048, 3);        // Но в случае с 3мя знаками, доступны только положительные (не хватает символов)
  delay(500);                         
}

ISR(TIMER2_A) {        // Прерывание Timer 2
  disp.tickManual();   // Обслуживание динамической индикации дисплея
}
