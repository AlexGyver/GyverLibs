/*
   Простой пример вывода чисел с плавающей точкой (float),с автоматическим выбором точности
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
  disp.displayFloatAuto(16.384);          // Вывод числа с автоматическим выбором точности
  delay(1000);                         
  disp.displayFloatAuto(8.192);           // Выведет с точностью 3 знака
  delay(1000);                         
  disp.displayFloatAuto(-4.096);          // Выведет с точностью 2 знака
  delay(1000);                         
  disp.displayFloatAuto(-12.048);         // Выведет с точносью 1 знак    
  delay(1000);                         
}

ISR(TIMER2_A) {        // Прерывание Timer 2
  disp.tickManual();   // Обслуживание динамической индикации дисплея
}
