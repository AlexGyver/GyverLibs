#ifndef TimeRandom_h
#define TimeRandom_h
#include <Arduino.h>

class TimeRandom {
  public:
    // установить канал (по умолч 0)
    void setChannel(byte channel) {
      _c = channel;
    }

    // обновить ЧМС
    void update(byte h, byte m, byte s) {
      _h = h;
      _m = m;
      _s = s;
    }

    // количество секунд с начала суток
    uint32_t getSec() {
      return (_h * 3600ul + _m * 60 + _s);
    }

    // количество минут с начала суток
    uint32_t getMin() {
      return (_h * 60 + _m);
    }

    // случайное число, обновляется каждые every секунд
    uint16_t fromSec(int every) {
      uint16_t s = getSec() / every;
      uint16_t val = (uint16_t)(_c + 1) * (_h + 1) * (_m + 1) * (s + 1);
      for (uint16_t i = 0; i < s & 0b1111; i++) val = (val * 2053ul) + 13849;
      return val;
    }

    // случайное число от 0 до max, обновляется каждые every секунд
    uint16_t fromSec(byte every, uint16_t max) {
      return ((uint32_t)max * fromSec(every)) >> 16;
    }

    // случайное число от min до max, обновляется каждые every секунд
    uint16_t fromSec(byte every, uint16_t min, uint16_t max) {
      return (fromSec(every, max - min) + min);
    }

    // случайное число, обновляется каждые every минут
    uint16_t fromMin(int every) {
      uint16_t m = getMin() / every;
      uint16_t val = (uint16_t)(_c + 1) * (_h + 1) * (m + 1);
      for (uint16_t i = 0; i < m & 0b1111; i++) val = (val * 2053ul) + 13849;
      return val;
    }

    // случайное число от 0 до max, обновляется каждые every минут
    uint16_t fromMin(byte every, uint16_t max) {
      return ((uint32_t)max * fromMin(every)) >> 16;
    }

    // случайное число от min до max, обновляется каждые every минут
    uint16_t fromMin(byte every, uint16_t min, uint16_t max) {
      return (fromMin(every, max - min) + min);
    }
  private:
    byte _h = 0, _m = 0, _s = 0, _c = 0;
};

#endif
