#include <GyverOLED.h>
// попробуй с буфером и без
GyverOLED<SSD1306_128x64, OLED_NO_BUFFER> oled;
// можно передать адрес: GyverOLED oled(0x3C);

char Lorem_ipsum[] = "Lorem ipsum dolor sit amet, лорем ипсум долор сит амет привет народ ё, consectetur adipiscing elit, sed do eiusmod tempor incididunt ut labore et dolore magna aliqua. Ut enim ad minim veniam, quis nostrud exercitation ullamco laboris nisi ut aliquip...";
void setup() {
  Serial.begin(9600);
  oled.init();              // инициализация

  // ускорим вывод, ВЫЗЫВАТЬ ПОСЛЕ oled.init()!!!
  Wire.setClock(400000L);   // макс. 800'000

  printScale(1);
  printScale(2);
  printScale(3);
  printScale(4);
  overlapTest();
  printTest();
  party();
  showText();
  scrollText();
  scaleText();
}

void loop() {
}

void printScale(byte x) {
  oled.clear();
  oled.setScale(x);
  for (byte i = 0; i < 8; i += x) {
    oled.setCursor(0, i);
    oled.print("Hello!");
  }
  delay(1000);
}

void party() {
  oled.clear();
  uint32_t tmr = millis();
  oled.setScale(3);
  oled.setCursor(10, 2);
  oled.print("ПЕННАЯ");
  oled.setScale(2);
  oled.setCursor(6, 5);
  oled.print("ВЕЧЕРИНКА!");
  oled.update();
  for (;;) {
    oled.invertDisplay(true);
    delay(200);
    oled.invertDisplay(false);
    delay(200);
    if (millis() - tmr > 5000) return;
  }
}

void overlapTest() {
  oled.clear();
  oled.setScale(1);
  oled.setCursorXY(0, 0);
  oled.print("LOL");
  oled.update();
  delay(500);
  oled.setCursorXY(0, 8);
  oled.print("KEK!");
  oled.update();
  delay(500);

  oled.setCursorXY(40, 4);
  oled.print("LOL");
  oled.update();
  delay(500);
  oled.setCursorXY(40, 12);
  oled.print("KEK!");
  oled.update();
  delay(500);

  oled.setScale(2);
  oled.setCursorXY(0, 24);
  oled.print("LOL");
  oled.update();
  delay(500);
  oled.setCursorXY(0, 40);
  oled.print("KEK!");
  oled.update();
  delay(500);

  oled.setCursorXY(60, 28);
  oled.print("LOL");
  oled.update();
  delay(500);
  oled.setCursorXY(60, 44);
  oled.print("KEK!");
  oled.update();
  delay(5000);
}

void printTest() {
  oled.clear();
  char data0[] = "Привет!";
  char data1 = 'b';
  int data2 = -50;
  float data3 = 1.25;
  oled.home();
  oled.setScale(1);
  oled.println(data0);
  oled.println(data1);
  oled.invertText(true);
  oled.println(data2);
  oled.println(data3);
  oled.invertText(false);
  oled.update();
  delay(5000);
}

void showText() {
  oled.clear();
  oled.home();
  oled.autoPrintln(true);
  oled.setScale(1);
  oled.print(Lorem_ipsum);
  oled.update();
  delay(5000);
}

void scrollText() {
  oled.clear();
  uint32_t tmr = millis();
  oled.autoPrintln(false);
  int val = 128;
  for (;;) {
    //oled.clear();   // ЗАКОММЕНТИРУЙ, ЕСЛИ ВКЛЮЧЕН БУФЕР
    oled.setScale(1);
    oled.setCursor(val, 0);
    oled.print(Lorem_ipsum);

    oled.setScale(2);
    oled.setCursor(val, 1);
    oled.print(Lorem_ipsum);

    oled.setScale(3);
    oled.setCursor(val, 4);
    oled.print(Lorem_ipsum);
    oled.update();
    val--;
    if (millis() - tmr > 5000) return;
  }
}

void scaleText() {
  oled.clear();
  uint32_t tmr = millis();
  byte scale = 1;
  oled.autoPrintln(true);
  for (;;) {
    oled.setScale(scale);
    oled.home();
    oled.print("Привет!");
    oled.update();
    delay(1000);
    oled.clear();
    scale++;
    if (scale > 4) scale = 1;
    if (millis() - tmr > 5000) return;
  }
}
