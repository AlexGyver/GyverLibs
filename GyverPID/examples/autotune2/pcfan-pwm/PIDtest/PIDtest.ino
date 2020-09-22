// тест коэффициентов из тюнера. Система: 4 пин вентилятор
#define PWM_PIN 3
#define TACH_PIN 2

#include "Tacho.h"
Tacho tacho;

#include "GyverPID.h"
// result: PI p: 0.57 PI i: 0.86  PID p: 0.90 PID i: 0.94 PID d: 0.14

GyverPID pid(0.9, 0.94, 0.14);
int period = 30;

void setup() {
  Serial.begin(9600);
  Serial.setTimeout(50);

  pinMode(PWM_PIN, OUTPUT);

  // пин тахометра вентилятора подтягиваем к VCC
  pinMode(TACH_PIN, INPUT_PULLUP);

  // настраиваем прерывание
  attachInterrupt(0, isr, FALLING);

  pid.setpoint = 200;
  pid.setDt(period);
}

// обработчик прерывания
void isr() {
  tacho.tick();   // сообщаем библиотеке об этом
}

void loop() {
  static uint32_t tmr;
  if (millis() - tmr >= period) {
    tmr = millis();
    pid.input = tacho.getRPM();   // даём регулятору обороты
    pid.getResult();              // считаем
    analogWrite(PWM_PIN, pid.output); // отправляем на ШИМ

    Serial.print(pid.input); Serial.print(' ');
    Serial.print(pid.output); Serial.print(' ');
    Serial.print(pid.integral); Serial.print(' ');
    Serial.println(pid.setpoint);
  }
  parsing();
}

// управление через плоттер
void parsing() {
  if (Serial.available() > 1) {
    char incoming = Serial.read();
    float value = Serial.parseFloat();
    switch (incoming) {
      case 'p': pid.Kp = value; break;
      case 'i': pid.Ki = value; break;
      case 'd': pid.Kd = value; break;
      case 's': pid.setpoint = value; break;
    }
  }
}
