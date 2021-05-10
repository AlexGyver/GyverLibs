//#define MAX_SPI_SPEED 500000	// дефайн для изменения скорости SPI, по умолч 1000000
#include "GyverMAX7219.h"

#define AM_W 4*8  // 4 матрицы (32 точки)
#define AM_H 2*8  // 2 матрицы (16 точек)

// подключение к аппаратному SPI (Нано CLK 13, DAT 11)
// указать количество МАТРИЦ по горизонтали и вертикали и CS пин
MAX7219 < AM_W / 8, AM_H / 8, 5 > mtrx; // W, H, CS

// подключение к любым пинам
//MAX7219 < AM_W / 8, AM_H / 8, 5, 11, 13 > mtrx; // W, H, CS, DATA, CLK

void setup() {
}

void loop() {
  //lines();
  //ball();
  //bezier();
  //bezier2();
  //bigBall();
  net();

  // переинициализация для больших матриц
  static uint32_t tmr;
  if (millis() - tmr >= 2000) {
    tmr = millis();
    mtrx.begin();
  }
}

void net() {
  const byte radius = 2;
  const byte amount = 5;
  static bool start = false;
  static int x[amount], y[amount];
  static int velX[amount], velY[amount];
  if (!start) {
    start = 1;
    for (byte i = 0; i < amount; i++) {
      x[i] = random(10, (AM_W - 1) * 10);
      y[i] = random(10, (AM_H - 1) * 10);
      velX[i] = random(2, 9);
      velY[i] = random(2, 9);
    }
  }
  mtrx.clear();
  for (byte i = 0; i < amount; i++) {
    x[i] += velX[i];
    y[i] += velY[i];
    if (x[i] >= (AM_W - 1 - radius) * 10 || x[i] < radius * 10) velX[i] = -velX[i];
    if (y[i] >= (AM_H - 1 - radius) * 10 || y[i] < radius * 10) velY[i] = -velY[i];
    mtrx.circle(x[i] / 10, y[i] / 10, radius);
  }

  for (int i = 0; i < amount; i++) {
    for (int j = 0; j < amount; j++) {
      if (i != j && dist(x[i] / 10, y[i] / 10, x[j] / 10, y[j] / 10) < 35) mtrx.line(x[i] / 10, y[i] / 10, x[j] / 10, y[j] / 10);
    }
  }
  mtrx.update();
  delay(10);
}
int dist(int x1, int y1, int x2, int y2) {
  int lx = (x2 - x1);
  int ly = (y2 - y1);
  return (sqrt(lx * lx + ly * ly));
}

void bezier2() {
  const byte amount = 3;
  static bool start = false;
  static int x[amount], y[amount];
  static int velX[amount], velY[amount];
  if (!start) {
    start = 1;
    for (byte i = 0; i < amount; i++) {
      x[i] = random(10, (AM_W - 1) * 10);
      y[i] = random(10, (AM_H - 1) * 10);
      velX[i] = random(2, 9);
      velY[i] = random(2, 9);
    }
  }
  mtrx.clear();
  byte bez[(amount + 1) * 2];
  for (byte i = 0; i < amount; i++) {
    x[i] += velX[i];
    y[i] += velY[i];
    if (x[i] >= (AM_W - 1) * 10 || x[i] < 0) velX[i] = -velX[i];
    if (y[i] >= (AM_H - 1) * 10 || y[i] < 0) velY[i] = -velY[i];
    mtrx.dot(x[i] / 10, y[i] / 10, 1);
    bez[i * 2] = x[i] / 10;
    bez[i * 2 + 1] = y[i] / 10;
  }
  bez[amount * 2] = bez[0];
  bez[amount * 2 + 1] = bez[1];

  mtrx.bezier(bez, amount + 1, 8);
  mtrx.update();
  delay(30);
}

void bigBall() {
  mtrx.clear();
  byte radius = 3;
  static int x = (AM_W / 2) * 10, y = (AM_H / 2) * 10;
  static int velX = 17, velY = 9;
  static bool fillFlag = 0;
  x += velX;
  y += velY;
  if (x >= (AM_W - 4) * 10 || x < radius * 10) {
    velX = -velX;
    fillFlag = !fillFlag;
  }
  if (y >= (AM_H - 4) * 10 || y < radius * 10) {
    velY = -velY;
    fillFlag = !fillFlag;
  }

  mtrx.circle(x / 10, y / 10, radius, fillFlag ? GFX_STROKE : GFX_FILL);
  mtrx.update();
  delay(20);
}

void bezier() {
  byte data[] = {0, 0, AM_W / 2, AM_H / 2, 0, AM_H - 1};
  for (int i = 0; i < AM_W; i++) {
    mtrx.clear();
    data[0] = data[4] = AM_W - i;
    data[2] = i;
    mtrx.bezier(data, 3, 6);
    mtrx.update();
    delay(30);
  }
  for (int i = AM_W; i > 0; i--) {
    mtrx.clear();
    data[0] = data[4] = AM_W - i;
    data[2] = i;
    mtrx.bezier(data, 3, 6);
    mtrx.update();
    delay(30);
  }
}

void lines() {
  mtrx.clear();
  for (byte i = 0; i < AM_W - 1; i += 3) {
    mtrx.line(0, 0, i, AM_H);
    mtrx.update();
    delay(30);
  }
  for (int i = AM_H - 1; i >= 0 ; i -= 3) {
    mtrx.line(0, 0, AM_W, i);
    mtrx.update();
    delay(30);
  }
  delay(100);

  mtrx.clear();
  for (int i = AM_W - 1; i > 0; i -= 3) {
    mtrx.line(AM_W - 1, 0, i, AM_H);
    mtrx.update();
    delay(30);
  }
  for (int i = 0; i < AM_H; i += 3) {
    mtrx.line(AM_W - 1, AM_H - 1, 0, i);
    mtrx.update();
    delay(30);
  }
  delay(100);
}

void ball() {
  mtrx.clear();
  static int x, y;
  static int velX = 17, velY = 9;
  x += velX;
  y += velY;
  if (x >= (AM_W - 1) * 10 || x < 0) velX = -velX;
  if (y >= (AM_H - 1) * 10 || y < 0) velY = -velY;

  mtrx.dot(x / 10, y / 10, 1);
  mtrx.dot(x / 10 + 1, y / 10 + 1, 1);
  mtrx.dot(x / 10 + 1, y / 10, 1);
  mtrx.dot(x / 10, y / 10 + 1, 1);
  mtrx.update();
  delay(20);
}
