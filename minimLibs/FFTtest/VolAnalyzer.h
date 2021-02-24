#pragma once
#include <Arduino.h>
#include "FastFilter.h"

class VolAnalyzer {
  public:
    VolAnalyzer (int pin = -1) {
      volF.setDt(20);
      volF.setPass(FF_PASS_MAX);
      maxF.setPass(FF_PASS_MAX);
      setVolK(25);
      setAmpliK(31);
      if (pin != -1) setPin(pin);
    }
    void setPin(int pin) {
      _pin = pin;
      pinMode(_pin, INPUT);
    }
    void setDt(int dt) {
      _dt = dt;
    }
    void setPeriod(int period) {
      _period = period;
    }
    void setVolDt(int volDt) {
      volF.setDt(volDt);
    }
    void setAmpliDt(int ampliDt) {
      _ampliDt = ampliDt;
    }
    void setWindow(int window) {
      _window = window;
    }
    void setVolK(byte k) {
      volF.setK(k);
    }
    void setAmpliK(byte k) {
      maxF.setK(k);
      minF.setK(k);
    }
    void setVolMin(int scale) {
      _volMin = scale;
    }
    void setVolMax(int scale) {
      _volMax = scale;
    }
    void setTrsh(int trsh) {
      _trsh = trsh;
    }
    bool tick(int thisRead = -1) {
      volF.compute();
      if (millis() - tmr3 >= _ampliDt) {    // период сглаживания амплитуды
        tmr3 = millis();
        maxF.setRaw(maxs);
        minF.setRaw(mins);
        maxF.compute();
        minF.compute();
        maxs = 0;
        mins = 1023;
      }
      if (_period == 0 || millis() - tmr1 >= _period) {  // период между захватом сэмплов
        if (_dt == 0 || micros() - tmr2 >= _dt) {        // период выборки
          tmr2 = micros();
          if (thisRead == -1) thisRead = analogRead(_pin);
          if (thisRead > max) max = thisRead; // ищем максимум
          if (!_first) {
            _first = 1;
            maxF.setFil(thisRead);
            minF.setFil(thisRead);
          }

          if (++count >= _window) {           // выборка завершена
            tmr1 = millis();
            raw = max;
            if (max > maxs) maxs = max;       // максимумы среди максимумов
            if (max < mins) mins = max;       // минимумы реди максимумов
            rawMax = maxs;
            maxF.checkPass(max);              // проверка выше максимума
            if (getMax() - getMin() < _trsh) max = 0; // если окно громкости меньше порого то 0
            else max = constrain(map(max, getMin(), getMax(), _volMin, _volMax), _volMin, _volMax); // перевод в громкость
            volF.setRaw(max);                         // фильтр столбика громкости
            if (volF.checkPass(max)) _pulse = 1;      // проверка выше максимума
            max = count = 0;
            return true;                              // выборка завершена
          }
        }
      }
      return false;
    }

    int getRaw() {
      return raw;
    }
    int getRawMax() {
      return rawMax;
    }
    int getVol() {
      return volF.getFil();
    }
    int getMin() {
      return minF.getFil();
    }
    int getMax() {
      return maxF.getFil();
    }
    bool getPulse() {
      if (_pulse) {
        _pulse = false;
        return true;
      }
      return false;
    }

  private:
    int _pin;
    int _dt = 500;      // 500 мкс между сэмплами достаточно для музыки
    int _period = 4;    // 4 мс между выборами достаточно
    int _ampliDt = 150;
    int _window = 20;   // при таком размере окна получаем длительность оцифровки вполне хватает
    uint32_t tmr1 = 0, tmr2 = 0, tmr3 = 0;
    int raw = 0;
    int rawMax = 0;
    int max = 0, count = 0;
    int maxs = 0, mins = 1023;
    int _volMin = 0, _volMax = 100, _trsh = 30;
    bool _pulse = 0, _first = 0;    
    FastFilter minF, maxF, volF;
};
