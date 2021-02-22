/*

  Быстрая и функциональная библиотка для модулей семисегментного дисплея TM74HC595
  построенных на базе одноименных сдвиговых регистров, без динамической индикации.
  Для обеспечения нормальной работы необходимо вручную обслуживать динамическую
  индикацию дисплея, вызывая метод .tickManual() с периодом 500-5000 мкс, используя 
  прерывания таймера или .tick() в чистом от блокирующих функций loop.
  
  - Обслуживание динамчиеской индикации встроенным таймером на micros()
  - Ручное обслуживание динамчиеской индикации при необходимости  
  - Поддержка чисел со знаком минус
  - Вывод int с нулями и без нулей слева
  - Вывод float с указанной точностью с нулями слева и без
  - Вывод float с автоматическим выбором точности
  - Вывод символа на нужную позицию, не трогает остальные знакоместа
  - Вывод массива символов (0 - 4 шт.) с выравниванием по левому / правому краю
  - Вывод 4х символов на дисплей вручную  
  - Вывод бегущих строк (блокирующая функция)
  - Управление любой из точек дисплея
  - Очистка дисплея
  - Обнаружение и вывод ошибки переполнения дисплея (-OVF и OVF)
  - Программная регулирвока яркости дисплея
  
  Краткое описание доступных методов смотрите ниже, в классе.
  
  Библиотека для простой реализации прерываний на аппаратных таймерах
  https://github.com/AlexGyver/GyverLibs/tree/master/GyverTimers
  
  V1.0 от 24/01/2021 by Egor 'Nich1con' Zakharov, специально для AlexGyver 
  
  ...
  
  Краткая документация:
    Disp595 (uint8_t dio, uint8_t sclk, uint8_t rclk);				// Конструктор дисплея
    void tick(void);												// Обслуживание динамической индикации со встроенным таймером на micros()
    void tickManual(void);											// Обслуживание динамической индикации вручную (для помещения в прерывание таймера и т.д.)  	
    void displayIntNulls(int16_t data);								// Вывод int с нулями слева
    void displayInt(int16_t data);									// Вывод int без нулей слева
	void displayFloatNulls(float data, uint8_t base);				// Вывод float с нулями слева, указанием кол-ва знаков (1 - 3)
    void displayFloatNulls(float data);							    // Вывод float с нулями слева, 1 знак после запятой 
	void displayFloat(float data, uint8_t base);				    // Вывод float без нулей слева, указанием кол-ва знаков (1 - 3) 
    void displayFloat(float data);				                    // Вывод float без нулей слева, 1 знак после запятой 
    void displayByte(uint8_t a, uint8_t b, uint8_t c, uint8_t d);	// Прямой вывод 4х байтов (символов) на дисплей
    void point(uint8_t pos, bool state);							// Добавление / удаление точки на позиции 0-3 (слева)
    void clear(void);												// Очистка дисплея		
	void brightness(uint8_t bright)									// Установка значения яркости (0 - 7), по умолчанию максимум (если обьявлена DISP595_BRIGHTNESS) 
	void runningString(uint8_t *str, uint8_t size, uint16_t ms)	    // Вывод бегущей строки размера size, с задержкой ms, внутри функции tick НЕ ОБСЛУЖИВАЕТСЯ
    void runningString(uint8_t *str, uint8_t size, uint16_t ms, bool useTick);	// Аналогично, но можно вручную включить / выключить встроенный в функцию tick()
	
  Дефайны, которые можно обьявить ПЕРЕД подключением библиотеки:
  DISP595_BRIGHTNESS -> Обьявить для подключения программной регулирвоки яркости дисплея
  DISP595_BRIGHTNESS_DEPTH число -> Глубина программной яркости, по умолчанию 8 градаций, больше - плавнее регулировка, сильнее мерацание дисплея без повышения частоты индикации.
  DISP595_FAST_PERIOD число -> Период в мкс динамической индикации во втроенном таймере метода tick для режима С ИСПОЛЬЗОВАНИЕМ РЕГУЛИРОВКИ ЯРКОСТИ 
  DISP595_SLOW_PERIOD число -> То же самое, но для стандартного режима (постоянная яркость)
  
  Константы:
  ALIGN_LEFT - выравнивание неполной строки по левому краю
  ALIGN_RIGHT - выравнивание неполной строки по правому краю
  
*/


#define ALIGN_LEFT  true
#define ALIGN_RIGHT false

#define digits(a) pgm_read_byte(&digs[a])
#define masks(a) pgm_read_byte(&msks[a])

#ifndef DISP595_BRIGHTNESS_DEPTH
#define DISP595_BRIGHTNESS_DEPTH 8
#endif

#ifndef DISP595_FAST_PERIOD
#define DISP595_FAST_PERIOD 500
#endif

#ifndef DISP595_SLOW_PERIOD
#define DISP595_SLOW_PERIOD 3500
#endif

#pragma once
#include <Arduino.h>          // Стандартная либа Arduino
#include <Disp595_symbols.h>  // Битовые маски для вывода символов

const uint8_t digs[] PROGMEM = {_0, _1, _2, _3, _4, _5, _6, _7, _8, _9};  // Массив цифр для вывода чисел
const uint8_t msks[] PROGMEM = {0b1000, 0b0100, 0b0010, 0b0001};          // Масив битовых маск для выбора знакоместа

class Disp595 {
  public:

    Disp595 (uint8_t dio, uint8_t sclk, uint8_t rclk) {
#ifdef __AVR__
      _clk_port = portOutputRegister(digitalPinToPort(sclk));
      _data_port = portOutputRegister(digitalPinToPort(dio));
      _latch_port = portOutputRegister(digitalPinToPort(rclk));
      _clk_mask = digitalPinToBitMask(sclk);
      _data_mask = digitalPinToBitMask(dio);
      _latch_mask = digitalPinToBitMask(rclk);
#else
      _clk_pin = sclk;
      _data_pin = dio;
      _latch_pin = rclk;
#endif
      pinMode(dio, OUTPUT);
      pinMode(sclk, OUTPUT);
      pinMode(rclk, OUTPUT);
    }


#ifdef DISP595_BRIGHTNESS
    void brightness(uint8_t bright) {
      softPwmDuty = constrain(bright, 0, DISP595_BRIGHTNESS_DEPTH - 1);
    }
#endif

    void tick(void) {
#ifdef DISP595_BRIGHTNESS
      if (micros() - dispTimer >= DISP595_FAST_PERIOD)
#else
      if (micros() - dispTimer >= DISP595_SLOW_PERIOD)
#endif
      {
        dispTimer = micros();
        tickManual();
      }
    }

    void tickManual(void) {
#ifdef DISP595_BRIGHTNESS
      if (softPwmCounter == softPwmDuty) {
        displayWrite(_empty, masks(dispPtr));
      }

      if (++softPwmCounter >= DISP595_BRIGHTNESS_DEPTH) {
        softPwmCounter = 0;
        if (++dispPtr > 3) dispPtr = 0;
        displayWrite(dispBuf[dispPtr], masks(dispPtr));
      }
#else
      displayWrite(dispBuf[dispPtr], masks(dispPtr)); // Вывод текущего знакоместа
      if (++dispPtr > 3) dispPtr = 0;                 // Сдвиг знакоместа
#endif
    }

    void displayIntNulls(int16_t data) {
      bool flag = false;                                          // Работа со знаком
      if (data < 0) {
        data *= -1;
        flag = true;
      }
      if (data > (flag ? 999 : 9999)) {                           // Работа с диапазоном
        displayBytes(flag ? _dash : _empty, _O, _V, _F);
        return;
      }

      dispBuf[0] = (flag ? _dash : digits((int16_t)data / 1000)); // Заполнение буфера напрямую цифрами
      dispBuf[1] = digits((int16_t)(data / 100) % 10);
      dispBuf[2] = digits((int16_t)(data / 10) % 10);
      dispBuf[3] = digits((int16_t)data % 10);
    }



    void displayInt(int16_t data) {
      bool flag = false;                  // Работа с минусом
      uint8_t buf = 0;

      if (data < 0) {
        data *= -1;
        flag = true;
      }

      if (data > (flag ? 999 : 9999)) {   // Отслеживание пределов
        displayBytes(flag ? _dash : _empty, _O, _V, _F);
        return;
      }

      clear();
      dispBuf[3] = digits((int16_t)data % 10);          // Крайний символ выводится всегда
      if (flag) dispBuf[2] = _dash;                     // Сразу ставим минус левее если входящее число меньше нуля
      if (data > 9) {                                   // Если знаков больше 1
        dispBuf[2] = digits((int16_t)(data / 10) % 10); // Заменяем минус (если он есть) на число
        if (flag)dispBuf[1] = _dash;                    // Следующий знак сразу заполняем минусом
      }
      if (data > 99) {                                  // Если знаков больше двух
        dispBuf[1] = digits((int16_t)(data / 100) % 10);// Заменяем минус на число
        if (flag)dispBuf[0] = _dash;                    // Сразу ставим минус на след позицию
      }
      if (data > 999) {                                 // Если знаков больше 3х и минуса нет (очевидно)
        dispBuf[0] = digits((int16_t)(data / 1000) % 10); // Заменяем последний знак цифрой
      }
    }


    void displayFloatNulls(float data, uint8_t base) {
      base = constrain(base, 1, 3);   // Аналогично проверка на пределы
      bool flag = false;

      if (data < 0) {                 // Работа с минусом
        data *= -1;
        flag = true;
      }

      uint16_t whole = floor(data);   // Подготовка целой и дробнйо части в целочисленном исполнении
      uint16_t fract = floor(((float)data - whole) * fastPow10(base));
      // Пример: 326.429 -> whole = 326, fract = 429;

      if (whole > fastPow10((flag ? 3 : 4) - base) - 1) { // Проверка на выползание за пределы
        displayBytes(flag ? _dash : _empty, _O, _V, _F);
        return;
      }

      uint8_t whole_digits[3];    // Буферы(а) для цифр
      uint8_t fract_digits[3];

      whole_digits[0] = digits(whole > 99 ? (int16_t)(whole / 100) % 10 : 0);  // Подготовка цифр
      whole_digits[1] = digits(whole > 9 ? (int16_t)(whole / 10) % 10 : 0);
      whole_digits[2] = digits((int16_t)whole % 10);

      fract_digits[0] = digits(fract > 99 ? (int16_t)(fract / 100) % 10 : 0);
      fract_digits[1] = digits(fract > 9 ? (int16_t)(fract / 10) % 10 : 0);
      fract_digits[2] = digits((int16_t)fract % 10);

      clear();
      switch (base) {                     // Вывод согласно точности
        case 1:
          dispBuf[0] = (flag ? _dash : whole_digits[0]);
          dispBuf[1] = whole_digits[1];
          dispBuf[2] = whole_digits[2];
          dispBuf[3] = fract_digits[2];
          break;
        case 2:
          dispBuf[0] = (flag ? _dash : whole_digits[1]);
          dispBuf[1] = whole_digits[2];
          dispBuf[2] = fract_digits[1];
          dispBuf[3] = fract_digits[2];
          break;
        case 3:
          dispBuf[0] = whole_digits[2];
          dispBuf[1] = fract_digits[0];
          dispBuf[2] = fract_digits[1];
          dispBuf[3] = fract_digits[2];
          break;
      }
      point(3 - base, true);
    }


    void displayFloatNulls(float data) {
      displayFloatNulls(data, 1);   // Дефолт вывод float - с одним дробным знаком, с нулями слева
    }


    void displayFloat(float data, uint8_t base) {
      base = constrain(base, 1, 3);   // Ограничение кол-ва дробных знаков
      bool flag = false;              // Флаг отрицательного числа

      if (data < 0) {         // Если число меньше нуля
        data *= -1;           // Сделать положительным
        flag = true;
      }

      uint16_t whole = floor(data);                                       // Получить целую часть в целочисленном виде
      uint16_t fract = floor(((float)data - whole) * fastPow10(base));    // Получить дробную часть в целочисленном виде
      // Пример: 326.429 -> whole = 326, fract = 429;

      if (whole > fastPow10((flag ? 3 : 4) - base) - 1) {                 // Если не хватает знаков на дисплее
        displayBytes(flag ? _dash : _empty, _O, _V, _F);                  // Выводим 'OVF'
        return;                                                           // Завершаем функцию
      }

      uint8_t whole_digits[3];  // Буфер для цифр целой части
      uint8_t fract_digits[3];  // Аналогично для дробной


      whole_digits[0] = (whole > 99 ? digits((uint16_t)(whole / 100) % 10) : _empty);   // Подготовка трех символов чисел
      whole_digits[1] = (whole > 9 ? digits((uint16_t)(whole / 10) % 10) : _empty);     // Для целой части
      whole_digits[2] = digits((int16_t)whole % 10);

      fract_digits[0] = digits(fract > 99 ? (int16_t)(fract / 100) % 10 : 0);    // Подготовка символов
      fract_digits[1] = digits(fract > 9 ? (int16_t)(fract / 10) % 10 : 0);      // Для дробной части
      fract_digits[2] = digits((int16_t)fract % 10);

      clear();        // Очистка дисплея
      switch (base) { // Разное кол-во знаков
        case 1:
          dispBuf[0] = (flag and whole > 9 ? _dash : whole_digits[0]); // Работаем с минусом при необходимости
          dispBuf[1] = (flag and whole < 9 ? _dash : whole_digits[1]); // Занимаем все знакоместа
          dispBuf[2] = whole_digits[2];
          dispBuf[3] = fract_digits[2];
          break;
        case 2:
          dispBuf[0] = (flag ? _dash : whole_digits[1]);
          dispBuf[1] = whole_digits[2];
          dispBuf[2] = fract_digits[1];
          dispBuf[3] = fract_digits[2];
          break;
        case 3:
          dispBuf[0] = whole_digits[2];
          dispBuf[1] = fract_digits[0];
          dispBuf[2] = fract_digits[1];
          dispBuf[3] = fract_digits[2];
          break;
      }
      point(3 - base, true);
    }


    void displayFloat(float data) {
      displayFloat(data, 1);    // Дефолт вывод float - с одним дробным знаком, без нулей слева
    }


    void displayFloatAuto(float data) {
      uint8_t base = 0;
      if (data > 0 and floor(fabs(data)) < 10) {              // Если число без минуса и целая часть меньше 10 - можно вывести 3 дробных знака
        base = 3;
      } else if (floor(fabs(data)) < (data > 0 ? 100 : 10)) { // Если есть минус или же целая часть в пределах 9 / 99 - выводим 2 знака
        base = 2;
      } else {                                                // Иначе выводим с 1 знаком, вылезания за пределы задетектит сама функция вывода float
        base = 1;
      }
      displayFloatNulls(data, base);                          // Выводим число с нужной точностью
    }


    void displayByte(uint8_t data, uint8_t pos) {
      dispBuf[pos] = data;   // Просто заполняем нужный элемент буфера символом
    }


    void displayBytes(uint8_t a, uint8_t b, uint8_t c, uint8_t d) {
      dispBuf[0] = a;                     // Прямой вывод 4х байт
      dispBuf[1] = b;
      dispBuf[2] = c;
      dispBuf[3] = d;
    }


    void displayBytes(uint8_t *str, uint8_t size, bool align = true) {
      size = constrain(size, 1, 4);                                       // Ограничение длинны массива
      for (uint8_t i = 0; i < size; i++) {
        dispBuf[(align ? i : 3 - i)] = str[(align ? i : (size - 1) - i)]; // Перенос массива в буфер дисплея
      }
    }


    void point(uint8_t pos, bool state) {
      bitWrite(dispBuf[pos], 7, !state);
    }


    void clear(void) {
      dispBuf[0] = _empty;
      dispBuf[1] = _empty;
      dispBuf[2] = _empty;
      dispBuf[3] = _empty;
    }


    void runningString(uint8_t *str, uint8_t size, uint16_t ms) {
      runningString(str, size, ms, false);
    }


    void runningString(uint8_t *str, uint8_t size, uint16_t ms, bool useTick) {
      uint8_t strBuf[size + 8];                  // Временный буфер для всей строки
      uint8_t strPtr = 0;
      uint32_t strTimer = millis();

      for (uint8_t i = 0; i < 4; i++) {               // Добавляем пустые символы в начало строки
        strBuf[i] = _empty;
      }
      for (uint8_t i = 0; i < size; i++) {            // Добавляем символы самой строки
        strBuf[i + 4] = str[i];
      }
      for (uint8_t i = size + 4; i < size + 8; i++) { // Добавляем пустые символы в конец
        strBuf[i] = _empty;
      }

      while (strPtr < size + 4) {
        if (useTick) tick();
        if (millis() - strTimer >= ms) {
          strTimer = millis();
          displayBytes(strBuf[strPtr], strBuf[strPtr + 1], strBuf[strPtr + 2], strBuf[strPtr + 3]);
          strPtr++;
        }
      }
    }


  private:
    volatile uint8_t dispBuf[4] = {0xFF, 0xFF, 0xFF, 0xFF};
    volatile uint8_t dispPtr = 0;
    uint32_t dispTimer = 0;

#ifdef DISP595_BRIGHTNESS
    volatile uint8_t softPwmDepth = 8;
    volatile uint8_t softPwmDuty = 7;
    volatile uint8_t softPwmCounter = 0;
#endif


    void displayWrite(uint8_t data, uint8_t mask) {
#ifdef __AVR__
      fastShiftOut(MSBFIRST, data);
      fastShiftOut(MSBFIRST, mask);
      *_latch_port |= _latch_mask;
      *_latch_port &= ~ _latch_mask;
#else
      shiftOut(_data_pin, _clk_pin, MSBFIRST, data);
      shiftOut(_data_pin, _clk_pin, MSBFIRST, mask);
      digitalWrite(_latch_pin, HIGH);
      digitalWrite(_latch_pin, LOW);
#endif
    }

    uint16_t fastPow10(uint8_t exp) {           // Быстрое возведение 10 в степень
      switch (exp) {
        case 1: return 10;
        case 2: return 100;
        case 3: return 1000;
      }
      return 1;
    }

#ifdef __AVR__
    void fastShiftOut(uint8_t bitOrder, uint8_t data) {
      for (uint8_t i = 0; i < 8; i++)  {
        if (bitOrder == MSBFIRST) {
          if (data & (1 << 7)) *_data_port |= _data_mask;
          else *_data_port &= ~_data_mask;
          data <<= 1;
        } else {
          if (data & 1) *_data_port |= _data_mask;
          else *_data_port &= ~_data_mask;
          data >>= 1;
        }
        *_clk_port |= _clk_mask;
        *_clk_port &= ~_clk_mask;
      }
    }

    volatile uint8_t *_clk_port;
    volatile uint8_t *_data_port;
    volatile uint8_t *_latch_port;
    uint8_t _clk_mask;
    uint8_t _data_mask;
    uint8_t _latch_mask;
#else
    uint8_t _clk_pin;
    uint8_t _data_pin;
    uint8_t _latch_pin;
#endif
};