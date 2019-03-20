/*
 * Данный скетч крутит 4 сервопривода с разными скоростями и ускорением
 */

#define AMOUNT 4  // кол-во серво

#include <ServoSmooth.h>
ServoSmooth servos[AMOUNT];

uint32_t servoTimer;
uint32_t turnTimer;

int position1 = 10;   // первое положение серв
int position2 = 160;  // второе положение серв
boolean flag;

void setup() {
  Serial.begin(9600);
  // подключаем
  servos[0].attach(2);
  servos[1].attach(3);
  servos[2].attach(4);
  servos[3].attach(5);

  // настраиваем макс. скорости и ускорения
  // скор. по умолч. 50
  // ускорение по умолч. 0.1
  servos[0].setSpeed(20);
  servos[1].setAccel(0.2);
  servos[2].setSpeed(100);
  servos[3].setAccel(0.05);
}

void loop() {
  // каждые 20 мс
  if (millis() - servoTimer >= 20) {
    servoTimer = millis();
    for (byte i = 0; i < AMOUNT; i++) {
      servos[i].tickManual();   // двигаем все сервы. Такой вариант эффективнее отдельных тиков
    }
  }

  // каждые 2 секунды
  if (millis() - turnTimer >= 2000) {
    turnTimer = millis();
    flag = !flag;
    for (byte i = 0; i < AMOUNT; i++) {
      if (flag) servos[i].setTargetDeg(position1);
      else servos[i].setTargetDeg(position2);
    }
  }
}
