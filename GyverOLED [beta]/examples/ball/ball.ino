// Пример GyverOLED. Смотри документацию на http://alexgyver.ru/gyveroled/
// пример со скачущим шариком

#define OLED_SOFT_BUFFER_64 // использовать буфер для 128x64 на стороне микроконтроллера
#include <GyverOLED.h>
GyverOLED oled;
// можно передать адрес: GyverOLED oled(0x3C);

void setup() {
  oled.init(OLED128x64);	// инициализация, указываем размер
  oled.setContrast(1);		// яркость 
  oled.clear();  			// очистить
}

void loop() {
  static byte ballSize = 10;
  static int posX = 50, posY = 50;
  static int velX = 5, velY = 3;

  oled.clear();
  oled.circle(posX, posY, ballSize, false);
  oled.update();
  posX += velX;
  posY += velY;
  if (posX < ballSize) {
    velX = -velX;
    posX = ballSize;
  }
  if (posX > 128 - ballSize - 1) {
    velX = -velX;
    posX = 128 - ballSize;
  }
  if (posY < ballSize) {
    velY = -velY;
    posY = ballSize;
  }
  if (posY > 64 - ballSize - 1) {
    velY = -velY;
    posY = 64 - ballSize;
  }
  delay(40);
}
