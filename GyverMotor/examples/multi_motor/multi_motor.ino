/*
	Пример управления двумя моторами
*/
#include "GyverMotor.h"
GMotor motor1(DRIVER2WIRE, 2, 3, HIGH);
GMotor motor2(DRIVER2WIRE, 4, 11, HIGH);
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
  motor1.setMode(FORWARD);
  motor2.setMode(FORWARD);
}

void loop() {
  // потенциометр на А0
  // преобразуем значение в -255.. 255
  int val_1 = 255 - analogRead(0) / 2;

  // потенциометр на А1
  // преобразуем значение в -255.. 255
  int val_2 = 255 - analogRead(1) / 2;

  motor1.setSpeed(val_1);
  motor2.setSpeed(val_2);
  // в данном случае мотор будет остановлен в среднем положении рукоятки
  // и разгоняться в противоположные скорости в крайних её положениях
  
  delay(10);  // задержка просто для "стабильности"
}
