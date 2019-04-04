// скетч управляет лентой, поддерживая яркость по мере разряда аккумуляторов
// R1 ближе к Vin, R2 к GND
/*
  (12V)                 (GND)
    |                     |
    |--[ R1 ]--|--[ R2 ]--|
               |          |
              (A1)      (GND)
*/

#include "GyverRGB.h"
GRGB strip(3, 5, 6);  // куда подключены цвета (R, G, B)

#define R1 100700   // 100 ком
#define R2 9999     // 10 ком
float dividerCoef = (float)((R1 + R2) / R2);
int voltage;

void setup() {
  Serial.begin(9600);
  strip.setDirection(REVERSE);
  strip.setHEX(WHITE);
  delay(2000);    // задержка для оценки яркости
}

void loop() {
  // получить напряжение с пина А0 (делитель) и перевести в милливольты
  voltage = (float)analogRead(1) * 5 / 1.023 * dividerCoef;

  // данный метод поддерживает яркость такой, какой она была бы при напряжении
  // питания 9000 милливольт (9 вольт), сравнивая его с текущим напряжением voltage
  // таким образом по мере разряда АКБ от 12 до 9 вольт яркость остаётся постоянной
  strip.constantBrightTick(9000, voltage);

  // по мере разряда "едут" цвета, данный метод чуть чуть исправляет ситуацию
  strip.gammaTick(voltage);
  delay(20);
}
