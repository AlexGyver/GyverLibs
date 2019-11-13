// подключаем "внешний" энкодер, для работы с расширителями пинов например

#define SW        0
#define DT        2
#define CLK       3

#include "GyverEncoder.h"
Encoder enc1;

void setup() {
  Serial.begin(9600);
  enc1.setType(TYPE2);
  pinMode(SW, INPUT_PULLUP);
  pinMode(CLK, INPUT);
  pinMode(DT, INPUT);
}

void loop() {
  // подаём значения напрямую в tick()
  // можно подавать лог. величины с любых расширителей пинов!!!
  // Здесь в качестве примера digitalRead
  enc1.tick(digitalRead(CLK), digitalRead(DT), !digitalRead(SW));

  if (enc1.isTurn()) {     // если был совершён поворот (индикатор поворота в любую сторону)
    // ваш код
  }

  if (enc1.isRight()) Serial.println("Right");         // если был поворот
  if (enc1.isLeft()) Serial.println("Left");

  if (enc1.isRightH()) Serial.println("Right holded"); // если было удержание + поворот
  if (enc1.isLeftH()) Serial.println("Left holded");

  if (enc1.isPress()) Serial.println("Press");         // нажатие на кнопку (+ дебаунс)
  if (enc1.isClick()) Serial.println("Click");         // отпускание кнопки (+ дебаунс)
  //if (enc1.isRelease()) Serial.println("Release");     // то же самое, что isClick

  if (enc1.isHolded()) Serial.println("Holded");       // если была удержана и энк не поворачивался
  //if (enc1.isHold()) Serial.println("Hold");         // возвращает состояние кнопки
}
