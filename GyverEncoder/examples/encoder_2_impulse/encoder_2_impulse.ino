/*
	В последнее время китайцы стали делать одинаковые модули (ку 40)
	с разными типами энкодеров - полный период и полпериода.
	Если ваш энкодер ведёт себя странно (один тик считает за два поворота),
	то смените тип энкодера
*/

#define CLK 4
#define DT 3
#define SW 2

#include "GyverEncoder.h"
Encoder enc1(CLK, DT, SW);

void setup() {
  Serial.begin(9600);
  enc1.setType(TYPE2);    // тип энкодера TYPE1 одношаговый, TYPE2 двухшаговый. Если ваш энкодер работает странно, смените тип
}

void loop() {
	// обязательная функция отработки. Должна постоянно опрашиваться
  enc1.tick();
  
  if (enc1.isRight()) Serial.println("Right");         // если был поворот
  if (enc1.isLeft()) Serial.println("Left");
  if (enc1.isRightH()) Serial.println("Right holded"); // если было удержание + поворот
  if (enc1.isLeftH()) Serial.println("Left holded");
}
