// пример работы с матрицей 32x8
#define M_PIN 8       // пин матрицы
#define M_WIDTH 32    // ширина матрицы
#define M_HEIGHT 8    // высота матрицы
#define NUM_LEDS (M_WIDTH * M_HEIGHT) // для удобства запомним и количство ледов

#include <microLED.h>
#include <FastLEDsupport.h>    // нужна для шума

// инициализация у матрицы такая же, как у ленты, но добавляются параметры в (скобках)
microLED<NUM_LEDS, M_PIN, MLED_NO_CLOCK, LED_WS2812, ORDER_GRB, CLI_AVER> matrix(M_WIDTH, M_HEIGHT, ZIGZAG, LEFT_TOP, DIR_DOWN);
// тип матрицы: ZIGZAG - зигзаг, PARALLEL - параллельная
// угол подключения: LEFT_BOTTOM - левый нижний, LEFT_TOP - левый верхний, RIGHT_TOP - правый верхний, RIGHT_BOTTOM - правый нижний
// направление ленты из угла подключения: DIR_RIGHT - вправо, DIR_UP - вверх, DIR_LEFT - влево, DIR_DOWN - вниз
// шпаргалка по настройке матрицы в папке docs в библиотеке

void setup() {
  matrix.setBrightness(100);  // яркость (0-255)

  // Проверка ориентации матрицы
  // Система координат - декартовая первая четверть (левый нижний угол - 0)
  // Левый нижний угол - жёлтый
  // Левый верхний - пурпурный
  // Правый нижний - голубой

  // для установки пикселя используем set(x, y, цвет)
  // всё остальное - так же как у ленты!!!

  matrix.set(0, 0, mYellow);
  matrix.set(0, 7, mPurple);
  matrix.set(7, 0, mTeal);
  matrix.show();
  delay(3000);
  matrix.clear();
}

void loop() {
  // примеры эффектов

  rainbow2D();
  //fire2D();
  //fire2D();
  //rainbow();    // горизонтальная радуга
  //balls();      // шарики
  //confetti();

  matrix.show();
  delay(30);
}

// =========== РАДУГА ===========
void rainbow() {
  static byte hue = 0;
  hue += 5;
  for (int i = 0; i < M_WIDTH; i++) {
    for (int j = 0; j < M_HEIGHT; j++)
      matrix.set(i, j, mWheel8(hue + i * 255 / M_WIDTH));
  }
}

// =========== ШАРИКИ ===========
#define BALLS_AMOUNT 5
boolean loadingFlag = true;
int coord[BALLS_AMOUNT][2];
int8_t vector[BALLS_AMOUNT][2];
mData ballColors[BALLS_AMOUNT];

void balls() {
  if (loadingFlag) {
    loadingFlag = false;
    for (byte j = 0; j < BALLS_AMOUNT; j++) {
      int sign;
      // забиваем случайными данными
      coord[j][0] = M_WIDTH / 2 * 10;
      random8(0, 2) ? sign = 1 : sign = -1;
      vector[j][0] = random8(4, 15) * sign;
      coord[j][1] = M_HEIGHT / 2 * 10;
      random8(0, 2) ? sign = 1 : sign = -1;
      vector[j][1] = random8(4, 15) * sign;
      ballColors[j] = mWheel8(random8(0, 9) * 28);
    }
  }

  matrix.clear();  // очистить

  // движение шариков
  for (byte j = 0; j < BALLS_AMOUNT; j++) {
    for (byte i = 0; i < 2; i++) {
      coord[j][i] += vector[j][i];
      if (coord[j][i] < 0) {
        coord[j][i] = 0;
        vector[j][i] = -vector[j][i];
      }
    }
    if (coord[j][0] > (M_WIDTH - 1) * 10) {
      coord[j][0] = (M_WIDTH - 1) * 10;
      vector[j][0] = -vector[j][0];
    }
    if (coord[j][1] > (M_HEIGHT - 1) * 10) {
      coord[j][1] = (M_HEIGHT - 1) * 10;
      vector[j][1] = -vector[j][1];
    }
    matrix.set(coord[j][0] / 10, coord[j][1] / 10, ballColors[j]);
  }
}

// =========== КОНФЕТТИ ===========
void confetti() {
  for (int i = 0; i < NUM_LEDS; i++) {
    if (matrix.get(i) == 0)
      if (random8(0, 100) == 0) matrix.set(i, mWheel8(random8()));
    matrix.fade(i, 30);
  }
}

// =========== ОГОНЬ ===========
mGradient<4> myGrad;
boolean loadingFlag2 = true;
void fire2D() {
  static int count = 0;
  if (loadingFlag2) {
    loadingFlag2 = false;
    // заполняем палитру
    myGrad.colors[0] = mBlack;
    myGrad.colors[1] = mRed;
    myGrad.colors[2] = mYellow;
    myGrad.colors[3] = mWhite;
  }
  for (int i = 0; i < M_WIDTH; i++)
    for (int j = 0; j < M_HEIGHT; j++)
      matrix.set(i, j, myGrad.get(inoise8(i * 50, j * 50, count), 255));
  count += 20;
}

// =========== РАДУЖНЫЕ ШТУКИ ===========
void rainbow2D() {
  static int count = 0;
  static byte count2 = 0;
  for (int i = 0; i < M_WIDTH; i++)
    for (int j = 0; j < M_HEIGHT; j++)
      matrix.set(i, j, mWheel8(count2 + inoise8(i * 50, j * 50, count), 255));
  count += 20;
  count2++;
}
