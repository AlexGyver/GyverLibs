#pragma once
#include <Arduino.h>

#define FF_SCALE 0

#define FF_PASS_MAX 1
#define FF_PASS_MIN 2

class FastFilter {
  public:
	FastFilter(byte k = 20, int dt = 0) {
		setK(k);
		setDt(dt);
	}
    void setK(byte k) {
      _k1 = k;
      _k2 = 32 - k;
    }
    void setDt(int dt) {
      _dt = dt;
    }
    void setPass(byte pass) {
      _pass = pass;
    }
    void setRaw(int raw) {
      _raw = raw;
    }
    void setFil(int fil) {
      _raw_f = fil;
    }
    bool checkPass(int val) {
      if (_pass == FF_PASS_MAX && val > _raw_f) {
        _raw_f = val;
        return 1;
      } else if (_pass == FF_PASS_MIN && val < _raw_f) {
        _raw_f = val;
        return 1;
      }
      return 0;
    }
    void compute() {
      if (_dt == 0 || millis() - _tmr >= _dt) {
        _tmr = millis();
        _raw_f = (_k1 * _raw_f + _k2 * _raw) >> 5;
      }
    }
    long getFil() {
      return _raw_f;
    }
    long getRaw() {
      return _raw;
    }
  private:
    uint32_t _tmr = 0;
    int _dt = 0;
    byte _k1 = 20, _k2 = 12;
    byte _pass = 0;
    int _raw_f = 0, _raw = 0;
};
