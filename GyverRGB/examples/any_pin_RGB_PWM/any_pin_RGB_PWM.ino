/*
   Пример управления RGB светодиодом из набора готовых цветов
   Светодиод/мосфеты подключены к ЛЮБЫМ ПИНАМ
*/
// НЕ ЗАБУДЬ включить в самой библиотеке параметр ALLOW_ANYPWM

#include "GyverRGB.h"

GRGB diode1(2, 3, 4, ANY_PWM);    // куда подключены цвета (R, G, B)
GRGB diode2(6, 7, 8, ANY_PWM);    // куда подключены цвета (R, G, B)

byte H;

void setup() {

}

void loop() {  
  H++;
  diode2.setHSV(H, 255, 255); 
  diode1.setHSV(H, 255, 255);
  delay(20);  
}
