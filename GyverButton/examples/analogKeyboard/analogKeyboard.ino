#include "GyverButton.h"
// аналоговая клавиатура подключена на А7. Схему смотри на странице библиотеки
// пин подключения не важен, лучше какой-нибудь ненужный
GButton myButt1(3, LOW_PULL, NORM_OPEN);
GButton myButt2(3, LOW_PULL, NORM_OPEN);
GButton myButt3(3, LOW_PULL, NORM_OPEN);

void setup() {
  Serial.begin(9600);
}

void loop() {
  int analog = analogRead(7);
  //Serial.println(analog);

  myButt1.tick(analog < 860 && analog > 820);
  myButt2.tick(analog < 740 && analog > 700);
  myButt3.tick(analog < 650 && analog > 600);

  if (myButt1.isHolded()) {
    Serial.println("hold 1");
  }
  if (myButt2.isHolded()) {
    Serial.println("hold 2");
  }
  if (myButt3.isHolded()) {
    Serial.println("hold 3");
  }
  
  delay(10);

}
