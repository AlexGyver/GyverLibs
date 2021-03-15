// тест коэффициентов из тюнера. Система: термистор и обогреватель ШИМ
#define MOS_PIN 3
#define THERM_PIN 0

#include "thermistorMinim.h"
thermistor therm(THERM_PIN, 10000, 3950);
// пин, сопротивление при 25 градусах (R термистора = R резистора!), бета-коэффициент

#include "GyverPID.h"
// result: PI p: 41.72 PI i: 4.76  PID p: 61.86  PID i: 9.02 PID d: 63.22
// result: PI p: 17.84 PI i: 0.92  PID p: 26.59  PID i: 1.61 PID d: 66.24

GyverPID pid(26.59, 1.61, 66.24);
int period = 500;

void setup() {
  Serial.begin(9600);
  Serial.setTimeout(50);
  pid.setpoint = 50;
  pid.setDt(period);
  pinMode(MOS_PIN, OUTPUT);
}

void loop() {
  static uint32_t tmr;
  if (millis() - tmr >= period) {
    tmr = millis();
    pid.input = therm.getTempAverage();
    pid.getResult();
    analogWrite(MOS_PIN, pid.output);

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
