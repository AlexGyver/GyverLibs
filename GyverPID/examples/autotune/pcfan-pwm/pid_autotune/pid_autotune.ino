// пример тюнера. Система: 4 пин вентилятор
#define PWM_PIN 3
#define TACH_PIN 2

#include "PIDtuner.h"
PIDtuner tuner;

#include "Tacho.h"
Tacho tacho;

void setup() {
  Serial.begin(9600);
  pinMode(PWM_PIN, OUTPUT);

  // пин тахометра вентилятора подтягиваем к VCC
  pinMode(TACH_PIN, INPUT_PULLUP);

  // настраиваем прерывание
  attachInterrupt(0, isr, FALLING);

  // направление, сигнал, ступенька, период стабилизации, точность стабилизации, продолж. импульса, период итерации
  tuner.setParameters(NORMAL, 130, 40, 2000, 1, 2000, 30);
}

// обработчик прерывания
void isr() {
  tacho.tick();   // сообщаем библиотеке об этом
}


void loop() {
  tuner.setInput(tacho.getRPM());   // даём тюнеру текущие обороты
  tuner.compute();                  // считаем
  analogWrite(PWM_PIN, tuner.getOutput());  // отправляем как ШИМ

  // выводит в порт текстовые отладочные данные, включая коэффициенты
  tuner.debugText();

  // выводит в порт данные для построения графиков, без коэффициентов
  //tuner.debugPlot();
}
