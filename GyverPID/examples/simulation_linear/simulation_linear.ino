// симуляция интегрирующего процесса
// (сигнал меняет скорость изменения величины, пример: линейный слайдер)
// для настройки коэффициентов вводи в плоттер букву и число:
// p - kp
// i - ki
// d - kd
// s - установка
// c - коэффициент среды
// пример: p5.5 - установит Kp 5.5

#define DT 30

#include "GyverPID.h"
GyverPID regulator(3, 6, 0, DT);

float value = 30;
float signal = 0;
float COEF = 0.1;

void setup() {
  Serial.begin(9600);
  Serial.setTimeout(100);
  regulator.setMode(ON_RATE);
  regulator.setDirection(NORMAL); // направление регулирования (NORMAL/REVERSE). ПО УМОЛЧАНИЮ СТОИТ NORMAL
  regulator.setLimits(0, 255);    // пределы (ставим для 8 битного ШИМ). ПО УМОЛЧАНИЮ СТОЯТ 0 И 255
  regulator.setpoint = 40;        // сообщаем регулятору температуру, которую он должен поддерживать
}

void loop() {
  // процесс и графики
  static uint32_t tmr;
  if (millis() - tmr >= DT) {
    tmr = millis();
    value += (signal - 128) * 0.05;   // процесс
    
    regulator.input = value;
    signal = regulator.getResult();
    if (regulator.integral < 0) regulator.integral = 0;

    Serial.print(value); Serial.print(' ');    
    Serial.print(signal); Serial.print(' ');
    //Serial.print(regulator.integral); Serial.print(' ');
    Serial.println(regulator.setpoint);    
  }

  // настройка
  if (Serial.available() > 1) {
    char incoming = Serial.read();
    float value = Serial.parseFloat();
    switch (incoming) {
      case 'p': regulator.Kp = value;
        break;
      case 'i': regulator.Ki = value;
        break;
      case 'd': regulator.Kd = value;
        break;
      case 's': regulator.setpoint = value;
        break;
      case 'c': COEF = value;
        break;
    }
  }
}
