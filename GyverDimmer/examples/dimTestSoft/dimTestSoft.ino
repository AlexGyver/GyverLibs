// тест одноканального диммера
// библиотека универсальная, поэтому требуется свой таймер
// в этом примере делаем на микросе!

class TimerUs {
  public:
    bool ready() {
      if (state && micros() - tmr >= prd) {
        tmr = micros();
        return true;
      }
      return false;
    }
    void stop() {
      state = false;
    }
    void restart() {
      tmr = micros();
      state = true;
    }
    void setPeriod(uint32_t period) {
      restart();
      prd = period;
      if (prd == 0) prd++;
    }

  private:
    bool state = 0;
    uint32_t tmr = 0, prd = 0;
};
TimerUs timer;

// zero cross на D2

#include <GyverDimmer.h>
Dimmer<D_PIN> dim;   // указать пин диммера
//Dimmer<D_PIN> dim(60);  // можно указать 60 Гц (умолч 50)

void setup() {
  // завести прерывание на детектор нуля
  attachInterrupt(0, isr, RISING);  // D2 == 0
}

void isr() {
  // вызывать в прерывании детектора нуля
  // если tickZero() - true - нужно перезапустить таймер с периодом getPeriod()
  if (dim.tickZero()) timer.setPeriod(dim.getPeriod());
  else timer.restart();
  // иначе перезапустить со старым
}

void loop() {
  // в "прерывании" таймера
  if (timer.ready()) {
    dim.tickTimer();    // вызвать tickTimer()
    timer.stop();       // остановить таймер
  }
  dim.write(analogRead(A0) / 4);  // принимает 0-255
}
