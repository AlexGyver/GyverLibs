// тест многоканального диммера
// библиотека универсальная, поэтому требуется свой таймер
// в этом примере делаем на аппаратном таймере
#include <GyverTimers.h>  // библиотека таймера

// zero cross на D2

#include <GyverDimmer.h>
DimmerMulti<2> dim;   // указать количество диммеров
//DimmerMulti<2> dim(60);  // можно указать 60 Гц (умолч 50)

void setup() {
  // завести прерывание на детектор нуля
  attachInterrupt(0, isr, RISING);  // D2 == 0

  // разрешаем прерывания по таймеру
  Timer2.enableISR();

  // настроить таймер на период в мкс (37 us для 50 гц сети)
  Timer2.setPeriod(dim.getPeriod());

  // подключить пины
  dim.attach(0, 4);   // канал 0, пин 4
  dim.attach(1, 5);   // канал 1, пин 5
}

// прерывание детектора нуля
void isr() {
  // вызывать в прерывании детектора нуля
  dim.tickZero();
  Timer2.restart();
}

// прерывание таймера
ISR(TIMER2_A) {
  dim.tickTimer();    // вызвать tickTimer()
}

void loop() {
  byte val = analogRead(A0) / 4;
  // врубаем, принимает 0-255
  dim.write(0, val);        // канал 0
  dim.write(1, 255 - val);  // канал 1
  delay(100);
}
