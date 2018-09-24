/*
   Пример управления RGB светодиодом плавная установка цвета
   Для УНО и НАНО это 3, 5, 6, 9, 10, 11
*/

#include "GyverRGB.h"
GRGB diode(6, 5, 3);  // куда подключены цвета (R, G, B)

/*
  Доступные цвета для fadeTo (HEX)
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
  diode.fadeTo(RED, 500);
  diode.fadeTo(BLACK, 500);
  diode.fadeTo(LIME, 500);
  diode.fadeTo(BLACK, 500);
  diode.fadeTo(BLUE, 500);
  diode.fadeTo(BLACK, 500);
}
