#define CLK 4
#define DT 3
#define SW 2

#include "GyverEncoder.h"
Encoder enc1(CLK, DT, SW);

void setup() {
  Serial.begin(9600);
  enc1.setType(1);    // 0 - полушаговый энкодер, 1 - полношаговый
  
  // val1 - переменная изменения при повороте
  // val2 - переменная изменения при нажатии повороте
  // общая функция setCounters(int norm, int hold);
  enc1.setCounterNorm(0);
  enc1.setCounterHold(50);
  
  // установка шага при повороте и при нажатом повороте
  // общая функция setSteps(int norm_step, int hold_step);
  enc1.setStepNorm(1);
  enc1.setStepHold(1);

  // пределы изменения в каждом режиме
  enc1.setLimitsNorm(0, 100);
  enc1.setLimitsHold(0, 100);
}

void loop() {
	// обязательная функция отработки. Должна постоянно опрашиваться
  enc1.tick();
  
  if (enc1.isTurn()) {             // если был совершён поворот
    Serial.print(enc1.normCount);  // получить счётчик обычный
    Serial.print(" ");
    Serial.println(enc1.holdCount); // получить счётчик при нажатой кнопке
  }
  if (enc1.isRight()) Serial.println("Right");         // если был поворот
  if (enc1.isLeft()) Serial.println("Left");
  if (enc1.isRightH()) Serial.println("Right holded"); // если было удержание + поворот
  if (enc1.isLeftH()) Serial.println("Left holded");
  if (enc1.isPress()) Serial.println("Press");         // нажатие на кнопку (+ дебаунс)
  if (enc1.isRelease()) Serial.println("Release");     // отпускание кнопки (+ дебаунс)
  if (enc1.isHolded()) Serial.println("Holded");       // если была удержана и энк не поворачивался
  //if (enc1.isHold()) Serial.println("Hold");         // возвращает состояние кнопки

}
