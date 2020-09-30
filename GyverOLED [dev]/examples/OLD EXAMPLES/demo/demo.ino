// Пример GyverOLED. Смотри документацию на http://alexgyver.ru/gyveroled/
//#define OLED_SOFT_BUFFER_64 // использовать буфер на стороне микроконтроллера
#include <GyverOLED.h>
GyverOLED oled;
// можно передать адрес: GyverOLED oled(0x3C);

void setup() {
  // при инициализации можно установить частоту (скорость) шины
  // максимум 800 кГц, самый быстрый вывод!
  oled.init(OLED128x64, 800);
  oled.setContrast(1);	// яркость 0-255
  oled.clear();			// очистить
  oled.home();			// курсор в 0,0

  oled.scale1X();		// масштаб шрифта х1  
  oled.print("Привет, ");
  oled.inverse(1);		// пишем с инверсией
  oled.println("мир!0123");
  oled.inverse(0);

  oled.scale2X();		// масштаб шрифта х2
  oled.print("Привет, ");
  oled.inverse(1);
  oled.println("мир!012");
  
  delay(1000);

  oled.line(0, 0, 127, 63);		// линия х0, у0, х1, у1
  delay(1000);
  oled.line(127, 32, 0, 63);
  delay(1000);
  oled.rect(70, 20, 100, 30, 0);  // прямоугольник х0, у0, х1, у1, заливка NO_FILL, FILL, CLEAR
  delay(1000);
  oled.rect(70, 20, 100, 30, FILL);  // прямоугольник х0, у0, х1, у1, заливка NO_FILL, FILL, CLEAR
  delay(1000);
  oled.roundRect(50, 40, 100, 60, FILL);	// прямоугольник скруглённый х0, у0, х1, у1, заливка NO_FILL, FILL, CLEAR
  delay(1000);
  oled.circle(20, 60, 15, 0);	// окружность х0, у0, радиус, заливка NO_FILL, FILL, CLEAR

  oled.update();	// обновить (только при использовании буфера)
}

void loop() {

}
