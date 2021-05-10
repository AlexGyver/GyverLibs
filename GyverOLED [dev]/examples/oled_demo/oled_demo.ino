// дефайн перед подключением либы - использовать microWire (лёгкая либа для I2C)
//#define USE_MICRO_WIRE

// дефайн перед подключением либы - скорость SPI
//#define OLED_SPI_SPEED 4000000ul

#include <GyverOLED.h>

// инициализация:
// GyverOLED<модель, буфер, интерфейс, CS, DC, RST> oled;
// "по умолчанию" - можно не указывать

// модель дисплея:
// SSD1306_128x32
// SSD1306_128x64
// SSH1106_128x64 (ТОЛЬКО С БУФЕРОМ)

// буфер:
// OLED_NO_BUFFER (без буфера)
// OLED_BUFFER (с буфером на стороне МК) - по умолчанию

// интерфейс:
// OLED_I2C - по умолчанию
// OLED_SPI (указать пины CS, DC, RST/RES)

// примеры:
//GyverOLED<SSD1306_128x32, OLED_BUFFER> oled;
//GyverOLED<SSD1306_128x32, OLED_NO_BUFFER> oled;
//GyverOLED<SSD1306_128x64, OLED_BUFFER> oled;
//GyverOLED<SSD1306_128x64, OLED_NO_BUFFER> oled;
//GyverOLED<SSD1306_128x64, OLED_BUFFER, OLED_SPI, 8, 7, 6> oled;
GyverOLED<SSH1106_128x64> oled;

// для I2C можно передать адрес: GyverOLED oled(0x3C);

// битмап создан в ImageProcessor https://github.com/AlexGyver/imageProcessor
// с параметрами вывода vertical byte (OLED)
const uint8_t bitmap_32x32[] PROGMEM = {
  0x00, 0x00, 0x00, 0x00, 0x00, 0x80, 0xC0, 0xC0, 0xE0, 0xF0, 0x70, 0x70, 0x30, 0x30, 0x30, 0x20, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0xC0, 0xE0, 0xF0, 0xF0, 0x70, 0x30, 0x30, 0x20, 0x00, 0x00,
  0x00, 0x30, 0x78, 0xFC, 0x7F, 0x3F, 0x0F, 0x0F, 0x1F, 0x3C, 0x78, 0xF0, 0xE0, 0xC0, 0x80, 0x80, 0x80, 0x40, 0xE0, 0xF0, 0xF8, 0xFC, 0xFF, 0x7F, 0x33, 0x13, 0x1E, 0x1C, 0x1C, 0x0E, 0x07, 0x00,
  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x80, 0xC0, 0xE0, 0xF0, 0xF9, 0xF7, 0xEF, 0x5F, 0x3F, 0x7F, 0xFE, 0xFD, 0xFB, 0xF1, 0xE0, 0xC0, 0x80, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
  0x00, 0x00, 0x00, 0x00, 0x00, 0x0C, 0x1E, 0x33, 0x33, 0x1F, 0x0F, 0x07, 0x03, 0x01, 0x00, 0x00, 0x00, 0x00, 0x00, 0x01, 0x03, 0x07, 0x0F, 0x1F, 0x3F, 0x1F, 0x0E, 0x04, 0x00, 0x00, 0x00, 0x00,
};

void setup() {
  Serial.begin(9600);
  oled.init();  // инициализация

  // --------------------------
  // настройка скорости I2C
  //Wire.setClock(800000L);   // макс. 800'000

  // --------------------------
  oled.clear();   // очистить дисплей (или буфер)
  oled.update();  // обновить. Только для режима с буфером! OLED_BUFFER

  // --------------------------
  oled.home();            // курсор в 0,0
  oled.print("Hello!");   // печатай что угодно: числа, строки, float, как Serial!
  oled.update();
  delay(2000);

  // --------------------------
  oled.setCursor(5, 1);   // курсор в (пиксель X, строка Y)
  oled.setScale(2);
  oled.print("Hello!");
  oled.update();
  delay(2000);

  // --------------------------
  oled.setCursorXY(15, 30); // курсор в (пиксель X, пиксель Y)
  oled.setScale(3);
  oled.invertText(true);    // инвертируй текст!
  oled.print("Привет!");
  oled.update();
  delay(2000);

  // --------------------------
  oled.clear();
  oled.home();
  oled.setScale(1);
  oled.invertText(false);
  oled.autoPrintln(true);   // автоматически переносить текст
  oled.print(F("Lorem ipsum dolor sit amet, лорем ипсум долор сит амет привет народ ё, consectetur adipiscing elit, sed do eiusmod tempor incididunt ut labore et dolore magna aliqua. Ut enim ad minim veniam"));
  oled.update();
  delay(2000);

  // --------------------------
  oled.home();
  oled.textMode(BUF_ADD);
  // BUF_ADD - наложить текст
  // BUF_SUBTRACT - вычесть текст
  // BUF_REPLACE - заменить (весь прямоугольник буквы)
  oled.home();
  oled.setScale(3);
  oled.print("KEK!");
  oled.update();
  delay(2000);

  // --------------------------
  // СЕРВИС
  //oled.setContrast(10);   // яркость 0..15
  //oled.setPower(true);    // true/false - включить/выключить дисплей
  //oled.flipH(true);       // true/false - отзеркалить по горизонтали
  //oled.flipV(true);       // true/false - отзеркалить по вертикали
  //oled.isEnd();           // возвращает true, если дисплей "кончился" - при побуквенном выводе

  // --------------------------
  oled.clear();
  oled.dot(0, 0);     // точка на x,y
  oled.dot(0, 1, 1);  // третий аргумент: 0 выкл пиксель, 1 вкл пиксель (по умолч)
  oled.line(5, 5, 10, 10);        // линия x0,y0,x1,y1
  //oled.line(5, 5, 10, 10, 0);   // пятый аргумент: 0 стереть, 1 нарисовать (по умолч)
  oled.fastLineH(0, 5, 10);       // горизонтальная линия (y, x1, x2)
  //oled.fastLineH(0, 5, 10, 0);  // четвёртый аргумент: 0 стереть, 1 нарисовать (по умолч)
  oled.fastLineV(0, 5, 10);       // аналогично верт. линия (x, y1, y2)
  oled.rect(20, 20, 30, 25);      // прямоугольник (x0,y0,x1,y1)
  oled.rect(5, 35, 35, 60, OLED_STROKE);      // прямоугольник (x0,y0,x1,y1)
  // параметры фигуры:
  // OLED_CLEAR - очистить
  // OLED_FILL - залить
  // OLED_STROKE - нарисовать рамку
  oled.roundRect(50, 5, 80, 25, OLED_STROKE);  // аналогично скруглённый прямоугольник
  oled.circle(60, 45, 15, OLED_STROKE);        // окружность с центром в (x,y, с радиусом)
  oled.circle(60, 45, 5, OLED_FILL);           // четвёртый аргумент: параметр фигуры

  // битмап
  oled.drawBitmap(90, 16, bitmap_32x32, 32, 32, BITMAP_NORMAL, BUF_ADD);
  //oled.drawBitmap(90, 16, bitmap_32x32, 32, 32);  // по умолч. нормал и BUF_ADD
  // x, y, имя, ширина, высота, BITMAP_NORMAL(0)/BITMAP_INVERT(1), BUF_ADD/BUF_SUBTRACT/BUF_REPLACE
  
  oled.update();
}

void loop() {
}
