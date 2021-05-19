// тест многоканального диммера
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
DimmerMulti<2> dim;   // указать количество диммеров
//DimmerMulti<2> dim(60);  // можно указать 60 Гц (умолч 50)

void setup() {
  // завести прерывание на детектор нуля
  attachInterrupt(0, isr, RISING);  // D2 == 0

  // настроить таймер на период в мкс (37 us для 50 гц сети)
  timer.setPeriod(dim.getPeriod());

  // подключить пины
  dim.attach(0, 4);   // канал 0, пин 4
  dim.attach(1, 5);   // канал 1, пин 5
}

void isr() {
  // вызывать в прерывании детектора нуля
  dim.tickZero();
  timer.restart();
}

void loop() {
  byte val = analogRead(A0) / 4;
  // врубаем, принимает 0-255
  dim.write(0, val);        // канал 0
  dim.write(1, 255 - val);  // канал 1
  delay(100);
}

void yield() {
  // в "прерывании" таймера
  if (timer.ready()) dim.tickTimer();    // вызвать tickTimer()
}
