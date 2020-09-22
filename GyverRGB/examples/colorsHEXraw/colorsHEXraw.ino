/*
   Пример управления RGB светодиодом из набора готовых цветов
   Светодиод/мосфеты подключены к ШИМ пинам!
   Для УНО и НАНО это 3, 5, 6, 9, 10, 11
*/

#include "GyverRGB.h"
GRGB diode(6, 5, 3);  // куда подключены цвета (R, G, B)

void setup() {

}

void loop() {
  diode.setHEX(0xff0000);
  delay(1000);
  diode.setHEX(0x00ffff);
  delay(1000);
  diode.setHEX(0x00ff00);
  delay(1000);
  diode.setHEX(0xffff00);
  delay(1000);
  diode.setHEX(0x123456);
  delay(1000);  
}
