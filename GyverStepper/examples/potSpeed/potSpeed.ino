// установка скорости потенциометром

#include <GyverStepper.h>
GStepper<STEPPER4WIRE> stepper(2048, 5, 3, 4, 2);
// мотор с драйвером ULN2003 подключается по порядку пинов, но крайние нужно поменять местами
// то есть у меня подключено D2-IN1, D3-IN2, D4-IN3, D5-IN4, но в программе поменял 5 и 2

void setup() {
  stepper.setRunMode(KEEP_SPEED); // режим поддержания скорости
  stepper.setSpeedDeg(50);        // в градусах/сек
}

void loop() {
  stepper.tick();

  // сделаем таймер на 50 мс и будем опрашивать потенциометр
  // менять скорость чаще нет смысла
  static uint32_t tmr2;
  if (millis() - tmr2 > 50) {
    tmr2 = millis();

    // ставим новую скорость (-512.. 512 шагов в секунду)
    // будет крутиться в разные стороны
    stepper.setSpeed(512 - analogRead(0));
  }
}
