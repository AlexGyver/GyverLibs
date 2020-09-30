/*
	Пример управления мотором при помощи драйвера полного моста и потенциометра
*/
#include "GyverMotor.h"
GMotor motor(DRIVER2WIRE, 2, 3, HIGH);

// варианты инициализации в зависимости от типа драйвера:
// GMotor motor(DRIVER2WIRE, dig_pin, PWM_pin, (LOW / HIGH) )
// GMotor motor(DRIVER2WIRE_NO_INVERT, dig_pin, PWM_pin, (LOW / HIGH) )
// GMotor motor(DRIVER3WIRE, dig_pin_A, dig_pin_B, PWM_pin, (LOW/HIGH) )
// GMotor motor(RELAY2WIRE, dig_pin_A, dig_pin_B, (LOW/HIGH) )
/*
  DRIVER2WIRE - двухпроводной драйвер (направление + ШИМ)
  DRIVER2WIRE_NO_INVERT - двухпроводной драйвер, в котором при смене направления не нужна инверсия ШИМ
  DRIVER3WIRE - трёхпроводной драйвер (два пина направления + ШИМ)
  RELAY2WIRE - реле в качестве драйвера (два пина направления)

  dig_pin, dig_pin_A, dig_pin_B - любой цифровой пин МК
  PWM_pin - любой ШИМ пин МК
  LOW / HIGH - уровень драйвера. Если при увеличении скорости мотор наоборот тормозит - смени уровень
*/

void setup() {
  // установка программного deadtime на переключение направления, микросекунды
  // по умолчанию стоит 0: deadtime отключен
  // motor.setDeadtime(200);

  // ГЛОБАЛЬНАЯ смена направления вращения мотора
  // например чтобы FORWARD совпадал с направлением движения "вперёд" у машинки  
  motor.setDirection(REVERSE);
  motor.setDirection(NORMAL);   // умолч.

  // смена режима работы мотора
  motor.setMode(FORWARD);   // вперёд
  motor.setMode(BACKWARD);  // назад
  motor.setMode(BRAKE);  	// активный тормоз
  motor.setMode(STOP);      // стоп, холостой (мотор отключен)

  // смена уровня драйвера (аналогично при инициализации)
  // Если при увеличении скорости мотор наоборот тормозит - смени уровень
  motor.setLevel(LOW);
  motor.setLevel(HIGH);     // по умолч.
  
  
  // для работы в 10 бит необходимо также настроить ШИМ на 10 бит!!!
  // читай тут https://alexgyver.ru/lessons/pwm-overclock/
  // motor.setResolution(10);

  // минимальный сигнал (по модулю), который будет подан на мотор
  // Избавляет от ситуаций, когда мотор покоится и "пищит"
  motor.setMinDuty(150);

  // ключ на старт!
  motor.setMode(FORWARD);
}

void loop() {
  // потенциометр на А0
  // преобразуем значение в -255.. 255
  int val = 255 - analogRead(0) / 2;

  // установка скорости:
  // * (0..255) при ручном выборе направления (setMode: FORWARD/BACKWARD)
  // * (-255..255) при автоматическом (поставить setMode(FORWARD))
  // * (0..1023) в режиме 10 бит при ручном выборе направления (setMode: FORWARD/BACKWARD)
  // * (-1023..1023) в режиме 10 бит при автоматическом (поставить setMode(FORWARD))    
  
  motor.setSpeed(val);
  // в данном случае мотор будет остановлен в среднем положении рукоятки
  // и разгоняться в противоположные скорости в крайних её положениях
  
  delay(10);  // задержка просто для "стабильности"
}
