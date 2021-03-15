// симуляция инерционного процесса
// для настройки коэффициентов вводи в плоттер букву и число:
// p - kp
// i - ki
// d - kd
// s - установка
// c - коэффициент среды
// пример: p5.5 - установит Kp 5.5

#define DT 30

#include "GyverPID.h"
GyverPID regulator(0, 0, 0, DT);

float value = 15;
float signal = 0;
float COEF = 0.1;

void setup() {
  Serial.begin(9600);
  Serial.setTimeout(100);
  regulator.setDirection(NORMAL); // направление регулирования (NORMAL/REVERSE). ПО УМОЛЧАНИЮ СТОИТ NORMAL
  regulator.setLimits(0, 255);    // пределы (ставим для 8 битного ШИМ). ПО УМОЛЧАНИЮ СТОЯТ 0 И 255
  regulator.setpoint = 40;        // сообщаем регулятору температуру, которую он должен поддерживать
}

void loop() {
  /*
    // случайное изменение "резкости" охлаждения
    static uint32_t tmr1;
    if (millis() - tmr1 >= 10000) {
    tmr1 = millis();
    COEF = (float)random(10, 20) / 100; // 0.1-0.2
    }
  */
  /*
    // случайное изменение установки
    static uint32_t tmr1;
    if (millis() - tmr1 >= 10000) {
    tmr1 = millis();
    regulator.setpoint = random(20, 40);
    }
  */

  // процесс и графики
  static uint32_t tmr;
  if (millis() - tmr >= DT) {
    tmr = millis();

    regulator.input = value;
    signal = regulator.getResult();
    if (regulator.integral < 0) regulator.integral = 0;
    process();

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

#define LOW_VALUE 15      // к этому значению "остывает" система
#define SIGNAL_COEF 0.1   // сила сигнала
#define DELAY_AMOUNT 10   // задержка изменения

void process() {
  static float valueSpeed;
  static float signalSpeed;
  static bool firstFlag = false;
  static float delayArray[DELAY_AMOUNT];
  
  // сигнал == скорость нагрева
  // ограничивает сигнал его же значением и плавно к нему стремится
  signalSpeed += (signal - signalSpeed) * 0.003;

  // складываем скорость сигнала и скорость охлаждения
  // скорость охлаждения получаем как разность "температуры" и её нулевого значения
  valueSpeed = signalSpeed * SIGNAL_COEF + (LOW_VALUE - value) * COEF;

  if (!firstFlag) {
    firstFlag = true;
    for (int i = 0; i < DELAY_AMOUNT; i++) delayArray[i] = valueSpeed;
  }

  for (int i = 0; i < DELAY_AMOUNT - 1; i++) delayArray[i] = delayArray[i + 1];
  delayArray[DELAY_AMOUNT - 1] = valueSpeed;

  // прибавляем скорость (интегрируем)
  value += /*valueSpeed*/delayArray[0];
}
