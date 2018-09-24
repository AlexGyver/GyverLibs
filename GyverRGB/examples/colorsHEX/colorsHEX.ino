/*
   Пример управления RGB светодиодом из набора готовых цветов
   Светодиод/мосфеты подключены к ШИМ пинам!
   Для УНО и НАНО это 3, 5, 6, 9, 10, 11
*/

#include "GyverRGB.h"
GRGB diode(6, 5, 3);  // куда подключены цвета (R, G, B)

/*
  Доступные цвета для setHEX
WHITE   // белый
SILVER  // серебро
GRAY    // серый
BLACK   // чёрный
RED     // красный
MAROON  // бордовый
YELLOW  // жёлтый
OLIVE   // олива
LIME    // лайм
GREEN   // зелёный
AQUA    // аква
TEAL    // цвет головы утки чирка https://odesign.ru/teal-color/
BLUE    // голубой
NAVY    // тёмно-синий
PINK    // розовый
PURPLE  // пурпурный
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
