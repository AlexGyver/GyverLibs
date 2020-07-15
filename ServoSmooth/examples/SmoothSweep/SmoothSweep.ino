/*
   Данный код плавно двигает туда-сюда одной сервой (на пине 5)
   Документация: https://alexgyver.ru/servosmooth/
*/

#include <ServoSmooth.h>
ServoSmooth servo;

uint32_t tmr;
boolean flag;

void setup() {
  Serial.begin(9600);
  servo.attach(5);        // подключить
  servo.setSpeed(130);    // ограничить скорость
  servo.setAccel(0.1);   	  // установить ускорение (разгон и торможение)
}

void loop() {
  servo.tick();

  if (millis() - tmr >= 3000) {   // каждые 3 сек
    tmr = millis();
    flag = !flag;
    servo.setTargetDeg(flag ? 50 : 120);  // двигаем на углы 50 и 120
  }
}
