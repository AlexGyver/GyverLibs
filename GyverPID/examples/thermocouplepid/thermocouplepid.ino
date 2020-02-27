#include "max6675.h"
#define thermoCLK 10
#define thermoCS 11
#define thermoDO 12
MAX6675 thermocouple(thermoCLK, thermoCS, thermoDO);

#include "GyverPID.h"
GyverPID regulator(1, 0, 0, 200);

#define SETPOINT 80.0
float value = 30;
float signal = 0;

void setup() {
  Serial.begin(9600);
  Serial.setTimeout(50);
  regulator.setDirection(NORMAL); // направление регулирования (NORMAL/REVERSE). ПО УМОЛЧАНИЮ СТОИТ NORMAL
  regulator.setLimits(0, 255);    // пределы (ставим для 8 битного ШИМ). ПО УМОЛЧАНИЮ СТОЯТ 0 И 255
  regulator.setpoint = SETPOINT;
  delay(500);
}

void loop() {
  static uint32_t tmr;
  static bool flag;
  static byte val = 0;
  if (millis() - tmr > 300) {
    tmr = millis();
    /*flag = !flag;
      val = flag ? 100 : 0;
      analogWrite(3, val);
    */
    regulator.input = thermocouple.readCelsius();
    regulator.getResult();
    analogWrite(3, regulator.output);
    
    Serial.print(regulator.input);
    Serial.print(' ' );
    Serial.println(regulator.output);
  }
  if (Serial.available() > 2) {
    char incoming = Serial.read();
    float value = Serial.parseFloat();
    switch (incoming) {
      case 'p': regulator.Kp = value;
        break;
      case 'i': regulator.Ki = value;
        break;
      case 'd': regulator.Kd = value;
        break;
    }
  }
}
