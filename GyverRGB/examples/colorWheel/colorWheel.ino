/*
   Демонстрация работы метода colorWheel, который
   устанавливает цвет по значениям 0-1530,
   делая плавный переход красный-зелёный-синий-красный
   Откройте монитор/плоттер для наглядного представления алгоритма
*/

#include <GyverRGB.h>
GRGB diode(5, 6, 9);  // куда подключены цвета (R, G, B)

void setup() {
  Serial.begin(9600);
}

void loop() {
  diode.colorWheel(map(analogRead(0), 0, 1023, 0, 1530));

  Serial.println(
    String(diode.showR) + " " +
    String(diode.showG) + " " +
    String(diode.showB));

  delay(20);
}
