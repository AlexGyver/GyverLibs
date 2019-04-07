/*
  Простенький алгоритм "огня" - симуляция мерцаний пламени свечи/костра
  Откройте плоттер порта, чтобы посомтреть на случайность происходящего
*/

#include "GyverRGB.h"
GRGB strip(5, 6, 9);  // куда подключены цвета (R, G, B)

void setup() {
  Serial.begin(9600);
  strip.setDirection(REVERSE);
}

int hue, val;
boolean hueFlag;
float valK = 0.3;

void loop() {
  if (hueFlag) {
    hue += random(-1, 3);
    if (hue > random(12, 16)) hueFlag = false;
  } else {
    hue -= random(-1, 3);
    if (hue < random(0, 6)) hueFlag = true;
  }
  hue = constrain(hue, 3, 16);

  val = val * (1 - valK) + map(hue, 3, 16, 20, 255) * valK;

  Serial.println(String(hue) + " " + String(val));
  strip.setHSV(hue, 255, val);
  delay(random(10, 40));
}
