#include <GyverOLED.h>
GyverOLED<SSD1306_128x64/*, OLED_NO_BUFFER*/> oled;
// можно передать адрес: GyverOLED oled(0x3C);


void setup() {
  Serial.begin(9600);
  oled.init();              // инициализация
  Wire.setClock(400000L);
  oled.clear();
  oled.update();
/*
  byte textPos1 = 8;
  byte textPos2 = 32;

  oled.createBuffer(5, 0, 66, textPos2 + 8 + 2);

  oled.roundRect(5, textPos1 - 4, 65, textPos1 + 8 + 2, OLED_STROKE);
  oled.setCursorXY(10, textPos1);
  oled.print("SET MODE");

  oled.roundRect(5, textPos2 - 4, 65, textPos2 + 8 + 2, OLED_FILL);
  oled.setCursorXY(10, textPos2);
  oled.invertText(true);
  oled.print("SUCK COCK");

  oled.sendBuffer();
  oled.update();
  for (;;);
  oled.createBuffer(0, 0, 60, 7);
  oled.home();
  oled.print("hello");
  oled.line(0, 4, 40, 7);
  oled.sendBuffer();
  for (;;);
  circleModes();
  oled.update();
  for (;;);
*/
}

int posX = 64, posY = 32; // начальная позиция
int velX = 4, velY = 3;   // скорость
void loop() {
  // движение с отскоками. Заходим на половину за стенки! Для теста
  posX += velX;
  posY += velY;
  if (posX >= 128 - 16 || posX <= -16) velX = -velX;
  if (posY >= 64 - 16 || posY <= -16) velY = -velY;

  //oled.fill(255);
  oled.rect(posX, posY, posX + 32, posY + 32, OLED_FILL);
  oled.update();
  delay(30);
  oled.clear(posX, posY, posX + 32, posY + 32);
  //oled.update();
}

void lineFill() {
  for (byte y = 0; y < 64; y += 3) {
    oled.line(0, 0, 128, y, OLED_FILL);
    oled.update();
    //delay(10);
  }
  for (int x = 128; x >= 0; x -= 3) {
    oled.line(0, 0, x, 64, OLED_FILL);
    oled.update();
    //delay(10);
  }
  delay(1000);
}

void circleModes() {
  for (;;) {
    oled.clear();
    oled.fill(255);
    oled.createBuffer(64 - 20, 32 - 20, 64 + 20, 32 + 20, 255);
    oled.circle(64, 32, 20, OLED_CLEAR);
    oled.sendBuffer();
    oled.update();
    delay(800);

    oled.clear();
    oled.createBuffer(64 - 20, 32 - 20, 64 + 20, 32 + 20);
    oled.circle(64, 32, 20, OLED_FILL);
    oled.sendBuffer();
    oled.update();
    delay(800);

    oled.clear();
    oled.createBuffer(64 - 20, 32 - 20, 64 + 20, 32 + 20);
    oled.circle(64, 32, 20, OLED_STROKE);
    oled.sendBuffer();
    oled.update();
    delay(800);
  }
}
