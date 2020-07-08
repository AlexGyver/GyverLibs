/*
  Пример управления мотором при помощи драйвера полного моста и потенциометра
  на разогнанном ШИМе
*/

#include "GyverMotor.h"
GMotor motor(DRIVER2WIRE, 2, 3, HIGH);

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
  // разгоняем ШИМ на пинах 3 и 11 (atmega328) до 31 кГц
  // читай тут: https://alexgyver.ru/lessons/pwm-overclock/
  TCCR2B = 0b00000001;  // x1
  TCCR2A = 0b00000001;  // phase correct

  // ключ на старт!
  motor.setMode(FORWARD);
}

void loop() {
  // потенциометр на А0
  // преобразуем значение в -255.. 255
  int val = 255 - analogRead(0) / 2;

  motor.setSpeed(val);
  // в данном случае мотор будет остановлен в среднем положении рукоятки
  // и разгоняться в противоположные скорости в крайних её положениях

  delay(10);  // задержка просто для "стабильности"
}
