/*
   Пример управления RGB светодиодом в пространстве HSV
   (цвет, насыщенность, яркость).
   Потенциометры подключены к А0, А1 и А2
   Светодиод/мосфеты подключены к ШИМ пинам!
   Для УНО и НАНО это 3, 5, 6, 9, 10, 11
*/

#include "GyverRGB.h"
GRGB diode(6, 5, 3);  // куда подключены цвета (R, G, B)

void setup() {

}

void loop() {
  byte H = analogRead(0) / 4;
  byte S = analogRead(1) / 4;
  byte V = analogRead(2) / 4;
  diode.setHSV(H, S, V);
}
