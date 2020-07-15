/*
   Данный код плавно двигает туда-сюда одной сервой (на пине 5)
   Наблюдаем за графиком угла в плоттере (Инструменты/Плоттер по последовательному соединения)
   Документация: https://alexgyver.ru/servosmooth/
*/

#include <ServoSmooth.h>
ServoSmooth servo;

uint32_t tmr;
boolean flag;

void setup() {
  Serial.begin(9600);
  servo.attach(5);      // подключить
  servo.setSpeed(130);  // ограничить скорость
  servo.setAccel(0.3);    // установить ускорение (разгон и торможение)
  servo.setTargetDeg(100);
  //servo.setDirection(REVERSE);
  servo.write(180);
}

void loop() {
  Serial.print(servo.getCurrentDeg());
  Serial.print(' ');
  Serial.print(servo.getTargetDeg());
  Serial.print(' ');
  Serial.println(servo.tick() * 10);	// состояние серво третьим графиком
  delay(10);


  static uint32_t timer;
  if (millis() - timer > 3000) {
    static bool kek = false;
    kek = !kek;
    timer = millis();
    servo.setTargetDeg(kek ? 10 : 50);
  }
}
