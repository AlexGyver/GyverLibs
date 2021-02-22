/*
   Демонстрация всего функционала, вывод разных данных на дисплей TM74HC595
   Обслуживание динамической индикации берет на себя аппаратный Timer 2
   https://alexgyver.ru/lessons/timer-isr/
*/

#define DATA_PIN   11
#define SCLK_PIN   10
#define RCLK_PIN   12
#define DISP_PERIOD 3000  // Период динамической индикации в мкс (500-6000)
// #define DISP_PERIOD 500  // Если используется регулировка яркости, дергать tick() надо чаще!

// #define DISP595_BRIGHTNESS           // Использовать регулировку яркости
// #define DISP595_BRIGHTNESS_DEPTH 10  // Глубину яркости можно изменить (по умолч. 8), больше - плавнее яркость, но сильнее мерцание дисплея
        
#include <GyverTimers.h>
#include <SevenSegmentsDisp.h>

Disp595 disp(DATA_PIN, SCLK_PIN, RCLK_PIN);

void setup() {
  Timer2.setPeriod(DISP_PERIOD);  // Заводим Timer 2 на прерывания с нужным периодом
  Timer2.enableISR();             // Включаем прерывания Timer 2
}

void loop() {

  /* Вывод встроенных и кастомных символов, сырых данных на элеменыт дисплея */
  disp.displayBytes(_H, _E, _L, _P); // Прямой вывод данных (символов) на дисплей
  disp.point(0, true);               // ПОСЛЕ вывода данных можно добавить / удалить точки
  disp.point(3, true);               // Указываем позицию точки слева (0 - 3) и включаем / выключаем ее (true / false)
  delay(1000);

  /*
     Программное управление яркостью дисплея
     Чем больше глубина яркости - тем чаще нужно дергать tick()!
     (если DISP595_BRIGHTNESS обьявлен перед подключением)
  */
  /*
    disp.brightnessDepth(8); // Установка "глубины" яркости - количества градаций, по умолчанию 8 градаций

    for (uint8_t i = 7; i > 0; i--) {
      disp.brightness(i);               // Яркость в диапазоне 0-7
      delay(500);
    }

    delay(1000);

    for (uint8_t i = 0; i < 8; i++) {
      disp.brightness(i);               // Яркость в диапазоне 0-7
      delay(500);
    }
  */


  /* Вывод одиночных символов на нужную позицию СЛЕВА */
  disp.displayByte(_L, 3);           // Заменяем 3й символ с 'P' на 'L'
  delay(1000);


  /* Вывод массива символов на дисплей с выравниванием по левому / правому краю */
  uint8_t data[] = {_D, _A};
  disp.clear();
  disp.displayBytes(data, sizeof(data));                // Вывод массива символов с выравниванием по левому краю
  delay(1000);

  disp.clear();
  disp.displayBytes(data, sizeof(data), ALIGN_RIGHT);   // Вывод массива символов с выравниванием по правому краю
  delay(1000);

  disp.clear();
  disp.displayBytes(data, sizeof(data), ALIGN_LEFT);    // Вывод массива символов с выравниванием по левому краю
  delay(1000);


  /* Вывод бегущих строк */
  uint8_t str[] = {_1, _2, _3, _4, _5, _empty, _t, _E, _S, _t};
  disp.runningString(str, sizeof(str), 300); // Вывод бегущей строки, указываем массив символов, размер, задержку в мс


  /* Вывод целых чисел без нулей слева */
  for (int8_t i = -50; i < 50; i++) {
    disp.displayInt(i);
    delay(100);
  }


  /* Вывод целых чисел c нулями слева */
  for (int8_t i = 50; i > -50; i--) {
    disp.displayIntNulls(i);
    delay(100);
  }
  
  /* Вывод чисел с плавающей точкой без нулей слева */
  disp.displayFloat(-12.34);      // Без указания точности, дробных знаков - 1
  delay(1000);
  disp.displayFloat(-12.34, 1);   // Или же кол-во знаков можно ввести вручную, от 1 до 3
  delay(1000);
  disp.displayFloat(12.34, 2);    // Выводить можно как положительные, так и отрицательные числа
  delay(1000);
  disp.displayFloat(1.234, 3);    // Но при выводе 3х десятичных знаков, места под знак минуса не достаточно
  delay(1000);


  /* Вывод чисел с плавающей точкой с нулями слева */
  disp.displayFloat(-3.14);       // Тут все аналоично, но с нулями слева
  delay(1000);
  disp.displayFloat(-1.34, 1);
  delay(1000);
  disp.displayFloat(3.1415, 2);
  delay(1000);
  disp.displayFloat(3.1415, 3);
  delay(1000);


  /* Вывод числа + символов */
  for (uint8_t i = 0; i < 30; i++) {
    int value = random(0, 99);
    disp.displayInt(value);         // СНАЧАЛА выводим число int
    disp.displayByte(_t, 0);        // Пустые символы можем вручную заполнить символами
    disp.displayByte(_equal, 1);
    delay(200);
  }


  /* Вывод числа + массива символов */
  uint8_t strData[] = {_P, _H};
  disp.displayFloat(1.2, 1);                     // СНАЧАЛА выводим число float (заняли всего 2 символа)
  disp.displayBytes(strData, sizeof(strData));   // Пустые символы можем заполнить массивом символов
  delay(1000);

  /* Очистка дисплея */
  disp.clear();                    // Буфер дисплея очищается
  delay(3000);
}

ISR(TIMER2_A) {        // Прерывание Timer 2
  disp.tickManual();   // Обслуживание динамической индикации дисплея
}
