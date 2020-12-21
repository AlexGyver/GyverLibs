/*
	Пример управления мотором при помощи драйвера полного моста и потенциометра
  Для режимов следования к позиции и удержания скорости
*/
#include "AccelMotor.h"
AccelMotor motor(DRIVER2WIRE, 2, 3, HIGH);

// инициализация наследуется от GyverMotor
// варианты инициализации в зависимости от типа драйвера:
// AccelMotor motor(DRIVER2WIRE, dig_pin, PWM_pin, level)
// AccelMotor motor(DRIVER3WIRE, dig_pin_A, dig_pin_B, PWM_pin, level)
// AccelMotor motor(RELAY2WIRE, dig_pin_A, dig_pin_B, level)
/*
  DRIVER2WIRE - двухпроводной драйвер (направление + ШИМ)
  DRIVER3WIRE - трёхпроводной драйвер (два пина направления + ШИМ)
  RELAY2WIRE - реле в качестве драйвера (два пина направления)

  dig_pin, dig_pin_A, dig_pin_B - любой цифровой пин МК
  PWM_pin - любой ШИМ пин МК
  level - LOW / HIGH - уровень драйвера. Если при увеличении скорости мотор наоборот тормозит - смени уровень
*/

void setup() {
  Serial.begin(9600);
  // использую мотор JGA25
  // редуктор 1:21.3
  // энкодер 12 тиков на оборот
  motor.setRatio(21.3 * 12);

  // период интегрирования (по умолч. 20)
  motor.setDt(30);  // миллисекунды

  // установка максимальной скорости для режима ACCEL_POS
  motor.setMaxSpeedDeg(600);  // в градусах/сек
  //motor.setMaxSpeed(400); // в тиках/сек

  // установка ускорения для режима ACCEL_POS
  motor.setAccelerationDeg(300);  // в градусах/сек/сек
  //motor.setAcceleration(300);  // в тиках

  // минимальный (по модулю) ШИМ сигнал (при котором мотор трогается)
  motor.setMinDuty(50);

  // коэффициенты ПИД регулятора
  motor.kp = 2;   // отвечает за резкость регулирования.
  // При малых значениях сигнала вообще не будет, при слишком больших – будет трясти

  motor.ki = 0.2; // отвечает за коррекцию ошибки в течение времени
  motor.kd = 0.1; // отвечает за компенсацию резких изменений

  // установить зону остановки мотора для режима стабилизации позиции в тиках (по умолч. 8)
  motor.setStopZone(10);

  motor.setRunMode(PID_SPEED);

  // IDLE_RUN - tick() не управляет мотором. Может использоваться для отладки
  // ACCEL_POS - tick() работает в режиме плавного следования к целевому углу
  // PID_POS - tick() работает в режиме резкого следования к целевому углу
  // ACCEL_SPEED - tick() работает в режиме плавного поддержания скорости (с заданным ускорением)
  // PID_SPEED - tick() работает в режиме поддержания скорости по ПИД регулятору
}

void loop() {
  // потенциометр на А0
  // преобразуем значение в -255.. 255
  static float val;
  val += (255 - analogRead(0) / 2 - val) * 0.3; // фильтор

  // для режима PID_SPEED/ACCEL_SPEED
  motor.setTargetSpeedDeg(val * 4); // задаём целевую скорость в градусах/сек

  // обязательная функция. Делает все вычисления
  // принимает текущее значение с энкодера или потенциометра
  motor.tick(encTick(4));

  static uint32_t tmr = 0;
  if (millis() - tmr > 100) {   // таймер на 100мс для графиков
    tmr += 100;

    // отладка скорости (открой плоттер)
    Serial.print(motor.getTargetSpeedDeg());
    Serial.print(',');
    Serial.print(motor.getDuty());
    Serial.print(',');
    Serial.println(motor.getSpeedDeg());
  }
}

// читаем энкодер вручную, через digitalRead()
long encTick(byte pin) {
  static bool lastState;
  static long encCounter = 0;
  bool curState = digitalRead(pin);       // опрос
  if (lastState != curState) {            // словили изменение
    lastState = curState;
    if (curState) {                       // по фронту
      encCounter += motor.getState();     // запомнили поворот
    }
  }
  return encCounter;
}
