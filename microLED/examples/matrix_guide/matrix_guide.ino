// пример работы с модулем матрицы 8x8

#define M_WIDTH 8             // ширина матрицы
#define M_HEIGHT 8            // высота матрицы

#define ORDER_GRB       // порядок цветов ORDER_GRB / ORDER_RGB / ORDER_BRG
#define COLOR_DEBTH 2   // цветовая глубина: 1, 2, 3 (в байтах)
// на меньшем цветовом разрешении скетч будет занимать в разы меньше места,
// но уменьшится и количество оттенков и уровней яркости!

// ВНИМАНИЕ! define настройки (ORDER_GRB и COLOR_DEBTH) делаются до подключения библиотеки!
#include <microLED.h>
#define LED_PIN 8       // пин ленты

#define NUM_LEDS M_WIDTH * M_HEIGHT
LEDdata leds[NUM_LEDS];  // буфер ленты типа LEDdata

microLED matrix(leds, LED_PIN, M_WIDTH, M_HEIGHT, PARALLEL, LEFT_TOP, DIR_RIGHT);  // объект матрица
// тип матрицы: ZIGZAG - зигзаг, PARALLEL - параллельная
// угол подключения: LEFT_BOTTOM - левый нижний, LEFT_TOP - левый верхний, RIGHT_TOP - правый верхний, RIGHT_BOTTOM - правый нижний
// направление ленты из угла подключения: DIR_RIGHT - вправо, DIR_UP - вверх, DIR_LEFT - влево, DIR_DOWN - вниз
// шпаргалка по настройке матрицы в папке docs в библиотеке

void setup() {
  matrix.setBrightness(30);  // яркость (0-255)
  // Проверка ориентации матрицы
  // Система координат - декартовая первая четверть
  // Левый нижний угол - жёлтый
  // Левый верхний - пурпурный
  // Правый нижний - голубой

  // рисуем пиксель: setPix(x, y, цвет)
  // цвет в формате LEDdata, то есть:
  // mRGB(r, g, b);  // RGB 255, 255, 255
  // mWHEEL(color);  // цвета 0-1530
  // mHEX(color);    // HEX цвет
  // mHSV(h, s, v);  // HSV 255, 255, 255
  // mCOLOR(color);  // цвет

  matrix.setPix(0, 0, mCOLOR(YELLOW));
  matrix.setPix(0, 7, mCOLOR(PURPLE));
  matrix.setPix(7, 0, mCOLOR(TEAL));
  matrix.show();
  delay(1000);
  matrix.clear();
}

void loop() {
  // примеры эффектов

  //rainbow();    // горизонтальная радуга
  //matrixNeo();  // матрица
  //balls();      // шарики
  confetti();

  matrix.show();
  delay(50);
}

void rainbow() {
  static byte hue = 0;
  hue++;
  for (byte i = 0; i < M_WIDTH; i++) {
    LEDdata thisColor = mHSV((byte)(hue + i * float(255 / M_WIDTH)), 255, 255);
    for (byte j = 0; j < M_HEIGHT; j++)
      matrix.setPix(i, j, thisColor);
  }
}

void matrixNeo() {
  for (byte x = 0; x < M_WIDTH; x++) {
    // заполняем случайно верхнюю строку
    uint32_t thisColor = matrix.getColorHEX(x, M_HEIGHT - 1);
    if (thisColor == 0)
      matrix.setPix(x, M_HEIGHT - 1, mHEX(0x00FF00 * (random(0, 10) == 0)));
    else if (thisColor < 0x002000)
      matrix.setPix(x, M_HEIGHT - 1, mHEX(0));
    else
      matrix.setPix(x, M_HEIGHT - 1, mHEX(thisColor - 0x002000));
  }

  // сдвигаем всё вниз
  for (byte x = 0; x < M_WIDTH; x++) {
    for (byte y = 0; y < M_HEIGHT - 1; y++) {
      // красим пиксель цветом верхнего над ним
      matrix.setPix(x, y, matrix.getColor(x, y + 1));
    }
  }
}

#define BALLS_AMOUNT 5
boolean loadingFlag = true;
int coord[BALLS_AMOUNT][2];
int8_t vector[BALLS_AMOUNT][2];
LEDdata ballColors[BALLS_AMOUNT];

void balls() {
  if (loadingFlag) {
    loadingFlag = false;
    for (byte j = 0; j < BALLS_AMOUNT; j++) {
      int sign;

      // забиваем случайными данными
      coord[j][0] = M_WIDTH / 2 * 10;
      random(0, 2) ? sign = 1 : sign = -1;
      vector[j][0] = random(4, 15) * sign;
      coord[j][1] = M_HEIGHT / 2 * 10;
      random(0, 2) ? sign = 1 : sign = -1;
      vector[j][1] = random(4, 15) * sign;
      ballColors[j] = mHSV(random(0, 9) * 28, 255, 255);
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
    matrix.setPix(coord[j][0] / 10, coord[j][1] / 10, ballColors[j]);
  }
}

void confetti() {
  for (int i = 0; i < NUM_LEDS; i++) {
    if (matrix.getColor(i) == 0)
      if (random(0, 30) == 0) matrix.setLED(i, mHSV(random(0, 255), 255, 255));
    matrix.fade(i, 20);
  }
}
