/*
   Пример управления RGB лентой в пространстве HSV одним потенциомтером
   Выход ШИМ инвертирован (255-0) для драйвера
   Потенциометр подключен к А0
   Драйвер подключен к ШИМ пинам!
   Для УНО и НАНО это 3, 5, 6, 9, 10, 11
*/

#include "GyverRGB.h"
GRGB diode(6, 5, 3);  // куда подключены цвета (R, G, B)

void setup() {
  diode.setDirection(REVERSE);  // NORMAL / REVERSE - направление ШИМ
								// общий катод - NORMAL
								// общий анод - REVERSE
}

void loop() {
  byte H = analogRead(0) / 4;
  diode.setHSV(H, 255, 255);
}
