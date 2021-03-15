// тест коэффициентов из тюнера. Система: термистор и обогреватель ШИМ
#define MOS_PIN 3
#define THERM_PIN 0

#include "thermistorMinim.h"
thermistor therm(THERM_PIN, 10000, 3950);
// пин, сопротивление при 25 градусах (R термистора = R резистора!), бета-коэффициент

#include "GyverPID.h"
//analysis: 99%  PI p: 14.27 PI i: 0.82  PID p: 19.02  PID i: 1.58 PID d: 57.11
//analysis: 97%  PI p: 15.20  PI i: 0.94  PID p: 20.26  PID i: 2.19 PID d: 46.96


GyverPID pid(15.2, 0.82, 0);
int period = 500;

void setup() {
  Serial.begin(9600);
  Serial.setTimeout(50);
  pid.setpoint = 60;
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
