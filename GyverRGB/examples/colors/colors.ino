/*
   Пример управления RGB светодиодом из набора готовых цветов
   Светодиод/мосфеты подключены к ШИМ пинам!
   Для УНО и НАНО это 3, 5, 6, 9, 10, 11
    _____
   /     \
  |       |
  |       |
  |_______|
   | | | |
   | | | |
   | | | |
     | | |
       |
   B G   R
*/

#include "GyverRGB.h"
GRGB diode(6, 5, 3);  // куда подключены цвета (R, G, B)

/*
  Доступные цвета для setColor
  WHITE
  SILVER
  GRAY
  BLACK
  RED
  MAROON
  YELLOW
  OLIVE
  LIME
  GREEN
  AQUA
  TEAL
  BLUE
  NAVY
  PINK
  PURPLE
*/

void setup() {

}

void loop() {
  diode.setHEX(RED);
  delay(1000);
  diode.setHEX(YELLOW);
  delay(1000);
  diode.setHEX(LIME);
  delay(1000);
  diode.setHEX(AQUA);
  delay(1000);
  diode.setHEX(PINK);
  delay(1000);  
}
