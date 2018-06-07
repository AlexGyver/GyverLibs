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
  G_WHITE
  G_SILVER
  G_GRAY
  G_BLACK
  G_RED
  G_MAROON
  G_YELLOW
  G_OLIVE
  G_LIME
  G_GREEN
  G_AQUA
  G_TEAL
  G_BLUE
  G_NAVY
  G_PINK
  G_PURPLE
*/

void setup() {

}

void loop() {
  diode.setColor(G_RED);
  delay(1000);
  diode.setColor(G_YELLOW);
  delay(1000);
  diode.setColor(G_LIME);
  delay(1000);
  diode.setColor(G_AQUA);
  delay(1000);
  diode.setColor(G_PINK);
  delay(1000);  
}
