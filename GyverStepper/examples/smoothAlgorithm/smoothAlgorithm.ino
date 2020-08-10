// используем более плавный алгоритм движения. Макс. скорость ограничена до
// 7000 шаг/сек, алгоритм использует много процессорного времени!

// перед подключением библиотеки дефайним
#define SMOOTH_ALGORITHM

#include <GyverStepper.h>
GStepper<STEPPER4WIRE> stepper(2048, 2, 3, 4, 5);
// мотор с драйвером ULN2003 подключается по порядку пинов, но крайние нужно поменять местами
// то есть у меня подключено D2-IN1, D3-IN2, D4-IN3, D5-IN4, но в программе поменял 5 и 2

void setup() {
  Serial.begin(115200);

  // режим следования к целевй позиции
  stepper.setRunMode(FOLLOW_POS);

  // установка макс. скорости в шагах/сек
  stepper.setMaxSpeed(400);

  // установка ускорения в шагах/сек/сек
  stepper.setAcceleration(500);
}

void loop() {
  // просто крутим туды-сюды
  if (!stepper.tick()) {
    static bool dir;
    dir = !dir;
    stepper.setTarget(dir ? -400 : 400);
  }
}
