// пример тюнера. Система: термистор и обогреватель ШИМ
#define MOS_PIN 3
#define THERM_PIN 0

#include "thermistorMinim.h"
thermistor therm(0, 10000, 3950);
// пин, сопротивление при 25 градусах (R термистора = R резистора!), бета-коэффициент

#include "PIDtuner.h"
PIDtuner tuner;

void setup() {
  Serial.begin(9600);

  // направление, сигнал, ступенька, период стабилизации, точность стабилизации, продолж. импульса, период итерации
  tuner.setParameters(NORMAL, 50, 15, 5000, 0.08, 15000, 500);
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
