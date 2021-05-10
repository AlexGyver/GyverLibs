// всякие анимации
//#define OLED_SPI_SPEED 4000000ul
#include <GyverOLED.h>

//GyverOLED<SSD1306_128x32, OLED_BUFFER> oled;
//GyverOLED<SSD1306_128x32, OLED_NO_BUFFER> oled;
//GyverOLED<SSD1306_128x64, OLED_BUFFER> oled;
//GyverOLED<SSD1306_128x64, OLED_NO_BUFFER> oled;
//GyverOLED<SSD1306_128x64, OLED_BUFFER, OLED_SPI, 8, 7, 6> oled;
GyverOLED<SSH1106_128x64> oled;
// можно передать адрес: GyverOLED oled(0x3C);

void setup() {
  Serial.begin(9600);
  oled.init();
  Wire.setClock(800000L);   // макс. 800'000
}

void loop() {
  uint32_t tmr;

  tmr = millis();
  while (millis() - tmr < 5000) ball();

  tmr = millis();
  while (millis() - tmr < 5000) bezier();

  tmr = millis();
  while (millis() - tmr < 5000) bezier2();

  tmr = millis();
  while (millis() - tmr < 5000) circleModes();

  tmr = millis();
  while (millis() - tmr < 5000) net();

  tmr = millis();
  while (millis() - tmr < 5000) bigBall();

  tmr = millis();
  while (millis() - tmr < 5000) lines();
}

void bezier2() {
  const byte amount = 3;
  static bool start = false;
  static int x[amount], y[amount];
  static int velX[amount], velY[amount];
  if (!start) {
    start = 1;
    for (byte i = 0; i < amount; i++) {
      x[i] = random(10, (128 - 1) * 10);
      y[i] = random(10, (64 - 1) * 10);
      velX[i] = random(5, 20);
      velY[i] = random(5, 20);
    }
  }
  oled.clear();
  int bez[(amount + 1) * 2];
  for (byte i = 0; i < amount; i++) {
    x[i] += velX[i];
    y[i] += velY[i];
    if (x[i] >= (128 - 1) * 10 || x[i] < 0) velX[i] = -velX[i];
    if (y[i] >= (64 - 1) * 10 || y[i] < 0) velY[i] = -velY[i];
    oled.dot(x[i] / 10, y[i] / 10, 1);
    bez[i * 2] = x[i] / 10;
    bez[i * 2 + 1] = y[i] / 10;
  }
  bez[amount * 2] = bez[0];
  bez[amount * 2 + 1] = bez[1];

  oled.bezier(bez, amount + 1, 8);
  oled.update();
}

void bezier() {
  int data[] = {0, 0, 128 / 2, 64 / 2, 0, 64 - 1};
  for (int i = 0; i < 128; i++) {
    oled.clear();
    data[0] = data[4] = 128 - i;
    data[2] = i;
    oled.bezier(data, 3, 7);
    oled.update();
  }
  for (int i = 128; i > 0; i--) {
    oled.clear();
    data[0] = data[4] = 128 - i;
    data[2] = i;
    oled.bezier(data, 3, 7);
    oled.update();
  }
}

void ball() {
  oled.clear();
  static int x, y;
  static int velX = 5, velY = 8;
  x += velX;
  y += velY;
  if (x >= (128 - 1) * 10 || x < 0) velX = -velX;
  if (y >= (64 - 1) * 10 || y < 0) velY = -velY;

  oled.dot(x / 10, y / 10, 1);
  oled.dot(x / 10 + 1, y / 10 + 1, 1);
  oled.dot(x / 10 + 1, y / 10, 1);
  oled.dot(x / 10, y / 10 + 1, 1);
  oled.update();
  delay(10);
}

void net() {
  const byte radius = 3;
  const byte amount = 6;
  static bool start = false;
  static int x[amount], y[amount];
  static int velX[amount], velY[amount];
  if (!start) {
    start = 1;
    for (byte i = 0; i < amount; i++) {
      x[i] = random(10, (128 - 1) * 10);
      y[i] = random(10, (64 - 1) * 10);
      velX[i] = random(2, 9);
      velY[i] = random(2, 9);
    }
  }
  oled.clear();
  for (byte i = 0; i < amount; i++) {
    x[i] += velX[i];
    y[i] += velY[i];
    if (x[i] >= (128 - 1 - radius) * 10 || x[i] < radius * 10) velX[i] = -velX[i];
    if (y[i] >= (64 - 1 - radius) * 10 || y[i] < radius * 10) velY[i] = -velY[i];
    oled.circle(x[i] / 10, y[i] / 10, radius);
  }

  for (int i = 0; i < amount; i++) {
    for (int j = 0; j < amount; j++) {
      if (i != j && dist(x[i] / 10, y[i] / 10, x[j] / 10, y[j] / 10) < 35) oled.line(x[i] / 10, y[i] / 10, x[j] / 10, y[j] / 10);
    }
  }
  oled.update();
}

int dist(int x1, int y1, int x2, int y2) {
  int lx = (x2 - x1);
  int ly = (y2 - y1);
  return (sqrt(lx * lx + ly * ly));
}

void bigBall() {
  oled.clear();
  byte radius = 10;
  static int x = (128 / 2) * 10, y = (64 / 2) * 10;
  static int velX = 17, velY = 9;
  static bool fillFlag = 0;
  x += velX;
  y += velY;
  if (x >= (128 - radius) * 10 || x < radius * 10) {
    velX = -velX;
    fillFlag = !fillFlag;
  }
  if (y >= (64 - radius) * 10 || y < radius * 10) {
    velY = -velY;
    fillFlag = !fillFlag;
  }

  oled.circle(x / 10, y / 10, radius, fillFlag ? OLED_STROKE : OLED_FILL);
  oled.update();
}

void lines() {
  oled.clear();
  for (byte i = 0; i < 128 - 1; i += 3) {
    oled.line(0, 0, i, 64);
    oled.update();
  }
  for (int i = 64 - 1; i >= 0 ; i -= 3) {
    oled.line(0, 0, 128, i);
    oled.update();
  }
  delay(100);

  oled.clear();
  for (int i = 128 - 1; i > 0; i -= 3) {
    oled.line(128 - 1, 0, i, 64);
    oled.update();
  }
  for (int i = 64 - 1; i > 0; i -= 3) {
    oled.line(128 - 1, 0, 0, i);
    oled.update();
  }
  delay(100);
}

void circleModes() {
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
