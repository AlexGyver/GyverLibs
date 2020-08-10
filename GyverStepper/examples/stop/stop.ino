// пример работы stop()
// на графике будет видно, как сместилась установка и мотор к ней затормозил
#include <GyverStepper.h>
GStepper<STEPPER4WIRE> stepper(2048, 5, 3, 4, 2);
// мотор с драйвером ULN2003 подключается по порядку пинов, но крайние нужно поменять местами
// то есть у меня подключено D2-IN1, D3-IN2, D4-IN3, D5-IN4, но в программе поменял 5 и 2

void setup() {
  Serial.begin(115200);
  stepper.setMaxSpeed(400);
  stepper.setAcceleration(300);
  stepper.setRunMode(FOLLOW_POS);

  // отправляем мотор подальше
  stepper.setTarget(-2024);
}

void loop() {
  stepper.tick();

  // плавная остановка через 3 секунды
  static uint32_t tmr;
  static bool flag = false;
  if (!flag && millis() > 3000) {
    stepper.stop();
    flag = true;
  }

  // график установки и текущей позиции
  static uint32_t tmr2;
  if (millis() - tmr2 > 20) {
    tmr2 = millis();
    Serial.print(stepper.getTarget());
    Serial.print(',');
    Serial.println(stepper.getCurrent());
  }
}
