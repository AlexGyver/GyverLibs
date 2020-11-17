#pragma once

// класс тахометра v1.1
// встроенный медианный фильтр
// вызывай tick() в прерывании по фронту
// забирай getRPM() частоту в оборотах в минуту
// забирай getHz() частоту в Герцах
// забирай getPeriod() период в мкс

#define _TACHO_TICKS_AMOUNT 10	  // количество тиков для счёта времени
#define _TACHO_TIMEOUT 1000000	  // таймаут прерываний (мкс), после которого считаем что вращение прекратилось

class Tacho {
  public:
    void tick() {   // tachoTime - время в мкс каждых _TACHO_TICKS_AMOUNT тиков
      if (!ticks--) {
        ticks = _TACHO_TICKS_AMOUNT - 1;
        tachoTime = micros() - tachoTimer;
        tachoTimer += tachoTime;  //== tachoTimer = micros();
        ready = true;
      }
    }

    uint16_t getRPM() {
      if (ready) {	// если готовы новые данные
        ready = false;
        if (tachoTime != 0) rpm = (uint32_t)_TACHO_TICKS_AMOUNT * 60000000 / median3(tachoTime);
      }
      if (micros() - tachoTimer > _TACHO_TIMEOUT) rpm = 0;
      return rpm;
    }

    float getHz() {
      if (ready) {  // если готовы новые данные
        ready = false;
        if (tachoTime != 0) hz = (float)_TACHO_TICKS_AMOUNT * 1000000 / median3(tachoTime);
      }
      if (micros() - tachoTimer > _TACHO_TIMEOUT) hz = 0;
      return hz;
    }

    uint32_t getPeriod() {
      return median3(tachoTime);
    }

  private:
    // быстрая медиана
    long median3(long value) {
      buf[counter] = value;
      if (++counter > 2) counter = 0;
      if ((buf[0] <= buf[1]) && (buf[0] <= buf[2])) return (buf[1] <= buf[2]) ? buf[1] : buf[2];
      else {
        if ((buf[1] <= buf[0]) && (buf[1] <= buf[2])) return (buf[0] <= buf[2]) ? buf[0] : buf[2];
        else return (buf[0] <= buf[1]) ? buf[0] : buf[1];
      }
    }

    volatile uint32_t tachoTime = 100000;   // для плавного старта значений
    volatile uint32_t tachoTimer = micros();
    volatile int ticks = 0;
    volatile bool ready = false;
    uint32_t buf[3] = {100000, 100000, 100000}; // для плавного старта значений
    byte counter = 0;
    int rpm = 0;
    float hz = 0.0;
};
