// класс психрометра (определение влажности по сухому и мокрому термометру)
// AlexGyver, 2021
#ifndef Psychrometer_h
#define Psychrometer_h
// http://www.pogodaiklimat.ru/table.php
// https://alexgyver.ru/lessons/signal-encoding/

#define _PSY_PGM
#ifdef AVR
#define _PSY_PGM PROGMEM
#endif

// разрешение таблицы
//static const int PTable[] _PSY_PGM = {19, 51, 125, 286, 611, 1277, 2337, 4243, 7377};  // step 10, size 9
static const int PTable[] _PSY_PGM = {19, 42, 88, 176, 335, 611, 1072, 1817, 2983, 4755, 7377};  // step 8, size 11
//static const int PTable[] _PSY_PGM = {19, 31, 51, 81, 125, 199, 286, 421, 611, 872, 1225, 1704, 2337, 3167, 4243, 5623, 7377}; // step 5, size 17

#define _PSY_STEP 8
#define _PSY_SIZE 11
#define _PSY_MIN -40
#define _PSY_MAX 40

class Psychrometer {
  public:
    // можно указать давление и А
    Psychrometer(int p = 1000, float a = 0.0007947) {
      P = p;
      A = a;
    }
    
    // получить влажность из (темп. сух., темп. влажн.)
    float getHumidity(float dry, float wet) {
      return (getP(wet) - A * P * (dry - wet)) * 100.0 / getP(dry);
    }
    
    // калибровка А по (темп. сух., темп. влажн., реальная влажн.)
    void calibrate(float dry, float wet, float hum) {
      A = (getP(wet) - hum / 100.0 * getP(dry)) / P / (dry - wet);
    }
    
    float A = 0.0007947;  // постоянная психрометра
    int P = 1000;         // атм давление в ГПа

  private:
    float getP(float signal) {
      signal = constrain(signal, _PSY_MIN, _PSY_MAX);
      int i = map(int(signal), _PSY_MIN, _PSY_MAX - _PSY_STEP, 0, _PSY_SIZE - 2);
      int thisMin = _PSY_STEP * i + _PSY_MIN;
      int thisMax = thisMin + _PSY_STEP;
      return map_f(signal, thisMin, thisMax, table(i), table(i + 1)) / 100.0f;
    }
    int table(int i) {
#ifdef AVR
      return pgm_read_word(&PTable[i]);
#else
      return PTable[i];
#endif
    }
    float map_f(float x, int in_min, int in_max, int out_min, int out_max) {
      return (float)(x - in_min) * (out_max - out_min) / (in_max - in_min) + out_min;
    }
};
#endif
