// тест одного канала
// библиотека универсальная, поэтому требуется свой таймер
// в этом примере делаем на аппаратном таймере
#include <GyverTimers.h>  // библиотека таймера

#define D_PIN 5
// zero cross на D2

#include <GyverDimmer.h>
Dimmer<D_PIN> dim;   // указать пин диммера
//Dimmer<D_PIN> dim(60);  // можно указать 60 Гц (умолч 50)

void setup() {
  // завести прерывание на детектор нуля
  attachInterrupt(0, isr, RISING);  // D2 == 0

  // разрешаем прерывания по таймеру
  Timer2.enableISR();
}

// прерывание детектора нуля
void isr() {
  // вызывать в прерывании детектора нуля
  // если tickZero() - true - нужно перезапустить таймер с периодом getPeriod()
  if (dim.tickZero()) Timer2.setPeriod(dim.getPeriod());
  else Timer2.restart();
  // иначе перезапустить со старым
}

// прерывание таймера
ISR(TIMER2_A) {
  dim.tickTimer();    // вызвать tickTimer()
  Timer2.stop();      // останавливаем таймер
}

void loop() {
  dim.write(analogRead(A0) / 4);  // принимает 0-255
  delay(100);
}
