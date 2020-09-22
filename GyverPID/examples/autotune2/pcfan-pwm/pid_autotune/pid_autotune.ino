// пример тюнера. Система: 4 пин вентилятор
#define PWM_PIN 3
#define TACH_PIN 2

#include "PIDtuner2.h"
PIDtuner2 tuner;

#include "Tacho.h"
Tacho tacho;

void setup() {
  Serial.begin(9600);
  pinMode(PWM_PIN, OUTPUT);

  // пин тахометра вентилятора подтягиваем к VCC
  pinMode(TACH_PIN, INPUT_PULLUP);

  // настраиваем прерывание
  attachInterrupt(0, isr, FALLING);

  // направление, начальный сигнал, конечный, период плато, точность, время стабилизации, период итерации
  tuner.setParameters(NORMAL, 150, 200, 1000, 1, 50);
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
