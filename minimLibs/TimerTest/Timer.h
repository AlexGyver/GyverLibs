#ifndef Timer_h
#define Timer_h
#include <Arduino.h>
// Удобный класс таймера на millis()

class Timer {
  public:
    // период, 0 не запущен / 1 запущен, режим 0 период / 1 таймер
    Timer(uint32_t prd = 1000, bool state = 0, bool mode = 0) {
      _prd = prd;
      _state = state;
      _mode = mode;
    }
    void setTimerMode() {   // установить в режим таймера
      _mode = 1;
    }
    void setPeriodMode() {  // установить в режим периода
      _mode = 0;
    }
    void setTime(uint32_t prd) {  // установить время работы таймера
      _prd = prd;
    }

    void reset() {          // сбросить таймер
      _tmr = millis();
      upd();
    }
    void start() {          // запустить/перезапустить таймер
      reset();
      _state = true;
    }
    void resume() {         // продолжить после остановки
      _state = true;
      _tmr = millis() - _buf;
      upd();
    }
    void stop() {           // остановить таймер
      _state = false;
      upd();
    }
    void force() {          // принудительно переполнить таймер
      _tmr = millis() - _prd;
    }

    // в режиме периода однократно вернёт true при каждом периоде
    // в режиме таймера будет возвращать true при переполнении
    bool ready() {
      if (_state && elapsed()) {
        if (!_mode) _tmr += _prd;
        return true;
      }
      return false;
    }
    bool elapsed() {        // вернёт true при переполнении
      upd();
      return (_buf >= _prd);
    }
    bool active() {         // статус таймера (вкл выкл)
      return _state;
    }
    bool status() {         // статус таймера + не переполнился ли он
      return _state && !elapsed();
    }

    int32_t timeLeft() {    // остаток времени в мс
      return ready() ? 0 : (_prd - _buf);
    }
    uint8_t timeLeft8() {   // остаток времени в 0-255
      return ready() ? 0 : (255 - _buf * 255l / _prd);
    }
    uint16_t timeLeft16() { // остаток времени в 0-65535
      return ready() ? 0 : (65535 - _buf * 65535l / _prd);
    }

  private:
    void upd() {
      if (_state) _buf = millis() - _tmr;
    }
    uint32_t _tmr = 0, _prd = 1000, _buf = 0;
    bool _state = 0, _mode = 0;
};
#endif