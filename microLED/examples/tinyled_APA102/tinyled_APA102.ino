// пример с tinyLED, вывод через ПРОГРАММНЫЙ SPI (указываем пины)

// дефайны настроек, прописываются перед подключением либы
#define TLED_ORDER ORDER_BGR   // порядок цветов
#define TLED_CHIP LED_APA102   // чип светодиода ленты

// ===== 2 пин SPI ленты =====
// программный SPI, например LED_APA102
// по умолч. PORTB
// можно указать свой порт, если у тиньки больше 8 ног или это вообще атмега
// пин указывается при создании
// #define TLED_CLK_PORT PORTB
// #define TLED_CLK_DDR DDRB
// #define TLED_DAT_PORT PORTB
// #define TLED_DAT_DDR DDRB

// ===== Запрет прерываний =====
// можно указать приоритет прерываний (умолч CLI_LOW):
// #define TLED_CLI CLI_OFF  // прерывания не запрещаются
// #define TLED_CLI CLI_LOW  // запрещаются на время передачи байта
// #define TLED_CLI CLI_AVER // запрещаются на время передачи светодиода (3 байта)
// #define TLED_CLI CLI_HIGH // запрещаются на время передачи всей ленты

// ===== Яркость =====
// #define TLED_STATIC_BRIGHT  // отключает изменение общей яркости вывода и экономит 60 байт флэш

// либа
#include "tinyLED.h"
tinyLED<3, 5> strip; // для 2-пин лент (APA102)

#define NUMLEDS 50 // количество светодиодов (для циклов)

void setup() {
  // begin() - начать отправку (НУЖНО ТОЛЬКО ДЛЯ SPI ЛЕНТ ИЛИ CLI_HIGH)
  // end() - закончить отправку (НУЖНО ТОЛЬКО ДЛЯ SPI ЛЕНТ ИЛИ CLI_HIGH)
  // write() - отправить один байт
  // sendRGB(r,g,b) - отправить цвет на 1 диод
  // sendRGBW(r,g,b,w) - отправить на rgbw ленту (например WS6812)
  // send(mData) - отправить цвет в формате mData
  // sendBuffer(mData буфер, размер) - отправить внешний буфер на ленту
  // setBrightness(0-255) - яркость для всего вывода

  // каждый вывод "задвигает" указанный цвет
  // =================================================
  strip.clear(NUMLEDS);
  strip.setBrightness(150);
  strip.begin();
  strip.sendRGB(255, 255, 255); // отправить r,g,b на первый лед
  strip.send(mOrange);      // отправить оранжевый
  strip.send(mAqua);        // голубой
  strip.send(mPurple);      // и розовый
  strip.end();
  // леды загорятся вот так:
  // начало--розовый--аква--оранж--белый
  delay(5000);              // пауза

  // =================================================
  // зальём 50 ледов белым. Вывод начнётся с начала!!
  strip.begin();
  for (byte i = 0; i < 10; i++) strip.send(mYellow);
  strip.end();
  delay(5000);              // пауза

  // =================================================
  // зальём 50 ледов с чередованием жёлтый-чёрный
  strip.begin();
  for (byte i = 0; i < 50; i++) {
    strip.send(i % 2 == 0 ? mYellow : mBlack);
  }
  strip.end();
  delay(5000);              // пауза
}

void loop() {
  // =================================================
  // иииии бегущая радуга!
  static byte count;
  count++;
  strip.begin();
  for (int i = 0; i < NUMLEDS; i++) {
    strip.send(mWheel8(count + i * 255 / NUMLEDS)); // выводим радугу
  }
  strip.end();
  delay(30);
}
