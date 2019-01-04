// два энкодера

#include "GyverEncoder.h"
Encoder enc1(4, 3, 2);
Encoder enc2(7, 6, 5);

void setup() {
  Serial.begin(9600);
}

void loop() {
	// обязательная функция отработки. Должна постоянно опрашиваться
  enc1.tick();
  enc2.tick();
  
  if (enc1.isLeft()) Serial.println("enc 1 left");
  if (enc1.isRight()) Serial.println("enc 1 right");
  if (enc2.isLeft()) Serial.println("enc 2 left");
  if (enc2.isRight()) Serial.println("enc 2 right");
}
