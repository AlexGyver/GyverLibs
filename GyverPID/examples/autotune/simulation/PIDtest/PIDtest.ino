// тест коэффициентов из тюнера на синтетическом нагревательном процессе

#include "GyverPID.h"
GyverPID pid(0.7, 0.5, 0.2);
int period = 50;

void setup() {
  Serial.begin(9600);
  Serial.setTimeout(50);
  pid.setpoint = 40;
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
