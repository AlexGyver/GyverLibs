// крутим мотор туда-сюда плавно с ускорением

#include "GyverStepper.h"
// подключим три мотора
// у первого и второго управление EN не подключаем
GStepper<STEPPER2WIRE> stepper1(100, 2, 3);
GStepper<STEPPER2WIRE> stepper2(100, 4, 5);
GStepper<STEPPER2WIRE> stepper3(100, 6, 7, 8);

void setup() {
  // мотор 1 просто вращается
  stepper1.setRunMode(KEEP_SPEED);
  stepper1.setSpeed(300);

  // мотор 2 будет делать sweep по проверке tick
  stepper2.setRunMode(FOLLOW_POS);
  stepper2.setMaxSpeed(1000);
  stepper2.setAcceleration(300);

  // мотор 3 будет перемещаться на случайную позицию
  stepper3.setRunMode(FOLLOW_POS);
  stepper3.setMaxSpeed(1000);
  stepper3.setAcceleration(300);
  stepper3.autoPower(true);
  stepper3.enable();
}

void loop() {
  // первый мотор
  stepper1.tick();
  
  // второй крутим туды-сюды (-1000, 1000)
  if (!stepper2.tick()) {
    static bool dir;
    dir = !dir;
    stepper2.setTarget(dir ? -1000 : 1000);
  }

  // третий по таймеру
  // будет отключаться при остановке
  stepper3.tick();
  static uint32_t tmr;
  if (millis() - tmr > 5000) {   // каждые 5 секунд
    tmr = millis();
    stepper3.setTarget(random(0, 2000));  // рандом 0-2000
  }
}
