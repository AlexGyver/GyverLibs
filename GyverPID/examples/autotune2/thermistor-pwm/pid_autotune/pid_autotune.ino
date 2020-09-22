// пример тюнера. Система: термистор и обогреватель ШИМ
#define MOS_PIN 3
#define THERM_PIN 0

#include "thermistorMinim.h"
thermistor therm(0, 10000, 3950);
// пин, сопротивление при 25 градусах (R термистора = R резистора!), бета-коэффициент

#include "PIDtuner2.h"
PIDtuner2 tuner;

void setup() {
  Serial.begin(9600);

  // направление, начальный сигнал, конечный, период плато, точность, время стабилизации, период итерации
  tuner.setParameters(NORMAL, 0, 80, 6000, 0.05, 500);
}

void loop() {  
  tuner.setInput(therm.getTempAverage());
  tuner.compute();
  analogWrite(MOS_PIN, tuner.getOutput());

  // выводит в порт текстовые отладочные данные, включая коэффициенты
  tuner.debugText();

  // выводит в порт данные для построения графиков, без коэффициентов
  //tuner.debugPlot();
}
