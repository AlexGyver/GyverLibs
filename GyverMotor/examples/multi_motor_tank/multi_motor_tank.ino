/*
	Пример управления двумя моторами при помощи одного джойстика по танковой схеме
*/
#include "GyverMotor.h"
GMotor motorR(DRIVER2WIRE, 2, 3, HIGH);
GMotor motorL(DRIVER2WIRE, 4, 11, HIGH);
// используем оба ШИМа таймера 2 (пины 3 и 11)

// варианты инициализации в зависимости от типа драйвера:
// GMotor motor(DRIVER2WIRE, dig_pin, PWM_pin, (LOW / HIGH) )
// GMotor motor(DRIVER3WIRE, dig_pin_A, dig_pin_B, PWM_pin, (LOW/HIGH) )
// GMotor motor(RELAY2WIRE, dig_pin_A, dig_pin_B, (LOW/HIGH) )
/*
  DRIVER2WIRE - двухпроводной драйвер (направление + ШИМ)
  DRIVER3WIRE - трёхпроводной драйвер (два пина направления + ШИМ)
  RELAY2WIRE - реле в качестве драйвера (два пина направления)

  dig_pin, dig_pin_A, dig_pin_B - любой цифровой пин МК
  PWM_pin - любой ШИМ пин МК
  LOW / HIGH - уровень драйвера. Если при увеличении скорости мотор наоборот тормозит - смени уровень
*/

void setup() {
  // ключ на старт!
  motorR.setMode(FORWARD);
  motorL.setMode(FORWARD);
}

void loop() {
  // джойстик на А0 и А1
  int signalX = 255 - analogRead(0) / 2;
  int signalY = 255 - analogRead(1) / 2;

  // преобразуем по танковой схеме
  int dutyR = signalY + signalX;
  int dutyL = signalY - signalX;

  motorR.setSpeed(dutyR);
  motorL.setSpeed(dutyL);

  delay(10);  // задержка просто для "стабильности"
}
