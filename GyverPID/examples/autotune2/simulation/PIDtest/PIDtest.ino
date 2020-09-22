// тест коэффициентов из тюнера на синтетическом нагревательном процессе

#include "GyverPID.h"

// result: PI p: 0.54 PI i: 0.38  PID p: 0.89 PID i: 0.37 PID d: 0.37
GyverPID pid(0.89, 0.37, 0.37);
int period = 50;

void setup() {
  Serial.begin(9600);
  Serial.setTimeout(50);
  pid.setpoint = 50;
  pid.setDt(period);
}

void loop() {
  static uint32_t tmr;
  if (millis() - tmr >= period) {
    tmr = millis();
    pid.input = process(pid.output);
    pid.getResult();

    Serial.print(pid.input); Serial.print(' ');
    Serial.print(pid.output); Serial.print(' ');
    Serial.print(pid.integral); Serial.print(' ');
    Serial.println(pid.setpoint);
  }
  parsing();
}

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
