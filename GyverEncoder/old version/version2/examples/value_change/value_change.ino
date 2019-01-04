#define CLK 7
#define DT 8
#define SW 9

#include "GyverEncoder.h"
Encoder enc1(CLK, DT, SW);

int value = 0;

void setup() {
  Serial.begin(9600);
  enc1.setType(TYPE2);    // тип энкодера TYPE1 одношаговый, TYPE2 двухшаговый. Если ваш энкодер работает странно, смените тип
}

void loop() {
	// обязательная функция отработки. Должна постоянно опрашиваться
  enc1.tick();

  if (enc1.isRight()) value++;     	// если был поворот направо, увеличиваем на 1
  if (enc1.isLeft()) value--;	    // если был поворот налево, уменьшаем на 1
  
  if (enc1.isRightH()) value += 5; 	// если было удержание + поворот направо, увеличиваем на 5
  if (enc1.isLeftH()) value -= 5;	// если было удержание + поворот налево, уменьшаем на 5  

  if (enc1.isTurn()) {       // если был совершён поворот (индикатор поворота в любую сторону)
    Serial.println(value);  // выводим значение при повороте
  }
}
