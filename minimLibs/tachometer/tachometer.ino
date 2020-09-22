// управляем 4 пин вентилятором и измеряем обороты
#define PWM_PIN 3     // ШИМ пин
#define TACH_PIN 2    // пин тахометра

#include "Tacho.h"
Tacho tacho;

void setup() {
  Serial.begin(9600);
  pinMode(PWM_PIN, OUTPUT);

  // пин тахометра вентилятора подтягиваем к VCC
  pinMode(TACH_PIN, INPUT_PULLUP);

  // настраиваем прерывание
  attachInterrupt(0, isr, FALLING);

  // включаем вентиль на скорость 150 (из 255)
  analogWrite(PWM_PIN, 150);
}

// обработчик прерывания
void isr() {
  tacho.tick();   // сообщаем библиотеке об этом
}

void loop() {
  // выводим два раза в секунду
  static uint32_t tmr;
  if (millis() - tmr > 500) {
    tmr = millis();
    Serial.println(tacho.getRPM());
  }
}
