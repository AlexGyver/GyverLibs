// Пример GyverOLED. Смотри документацию на http://alexgyver.ru/gyveroled/
#define OLED_SOFT_BUFFER_64 // использовать буфер на стороне микроконтроллера
#define OLED_EXTFONTS  // указываем, что используются расширенные шрифты

#include <GyverOLED.h>
GyverOLED oled;
// можно передать адрес: GyverOLED oled(0x3C);

extern uint8_t ExtFont8x12[]; // подключаем шрифт
//extern uint8_t ExtFont8x13[];	
/*
  на выбор:
  ExtFont8x12	
  ExtFont8x13	
  ExtFont8x13OnlyRus
  ExtFont8x14	
  ExtFont8x14B	
*/

void setup() {
  oled.init(OLED128x64);
  oled.clear();
  oled.home();

  // устанавливаем шрифт в качестве текущего
  // можно подключить (выше где extern) несколько и переключаться между ними
  oled.setFont(ExtFont8x12);
  
  oled.print("Peace! Миру мир!");
  oled.print("Папа у Васи силён в математике! Учится папа за Васю весь год.");
  oled.update();
}

void loop() {
}
