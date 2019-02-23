// пример с установкой цвета как цветовой температуры в Кельвинах
// отправляем значение в Кельвинах в порт

#include "GyverRGB.h"
GRGB diode(6, 5, 3);  // куда подключены цвета (R, G, B)

void setup() {
  Serial.begin(9600);
}

void loop() {
  if (Serial.available()) {
    int buf = Serial.parseInt();
    Serial.println(buf);
    diode.setKelvin(buf);
  }
}
