// крутим мотор туда-сюда плавно с ускорением
// драйвер STEP-DIR

#include <GyverStepper.h>
GStepper<STEPPER2WIRE> stepper(800, 2, 3, 4);
// 2 - STEP
// 3 - DIR
// 4 - EN

void setup() {
  // режим следования к целевй позиции
  stepper.setRunMode(FOLLOW_POS);

  // установка макс. скорости в шагах/сек
  stepper.setMaxSpeed(1000);

  // установка ускорения в шагах/сек/сек
  stepper.setAcceleration(1000);
}

int val = 2000;
void loop() {
  if (!stepper.tick()) {
    val = -val;
    stepper.setTarget(val);
  }
}
