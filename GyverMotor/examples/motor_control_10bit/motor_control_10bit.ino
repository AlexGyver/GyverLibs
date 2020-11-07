/*
  Пример управления мотором при помощи драйвера полного моста и потенциометра
  на ШИМ 10 бит
*/

#include "GyverMotor.h"
GMotor motor(DRIVER2WIRE, 2, 9, HIGH);

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
  // разгоняем ШИМ на пинах 9 и 10 (atmega328) до 16 кГц 10 бит
  // читай тут: https://alexgyver.ru/lessons/pwm-overclock/
  TCCR1A = 0b00000011;  // 10bit
  TCCR1B = 0b00001001;  // x1 fast pwm

  // активируем 10-битный режим библиотеки
  motor.setResolution(10);

  // ключ на старт!
  motor.setMode(FORWARD);
}

void loop() {
  // потенциометр на А0
  // преобразуем значение в -1023.. 1023
  int val = 1023 - analogRead(0) * 2;

  motor.setSpeed(val);
  // в данном случае мотор будет остановлен в среднем положении рукоятки
  // и разгоняться в противоположные скорости в крайних её положениях

  delay(10);  // задержка просто для "стабильности"
}
