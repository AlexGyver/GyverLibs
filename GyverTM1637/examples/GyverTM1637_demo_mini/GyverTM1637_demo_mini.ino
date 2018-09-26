/*
   Пример вывода на дисплей с регистром TM1637
   показывает все возможности библиотеки GyverTM1637
   AlexGyver Technologies http://alexgyver.ru/
*/

#define CLK 2
#define DIO 3

#include "GyverTM1637.h"
GyverTM1637 disp(CLK, DIO);

uint32_t Now, clocktimer;
boolean flag;

void setup() {
  Serial.begin(9600);
  disp.clear();
  disp.brightness(7);  // яркость, 0 - 7 (минимум - максимум)

}

void loop() {
  runningText();
  normClock();
  scrollClock();
  twistClock();
  scrolls();
  twists();  
  ints();
  fadeBlink();
}

void twists() {
  disp.clear();
  delay(200);
  for (byte i = 0; i < 10; i++) {
    disp.twist(3, i, 20);
    delay(300);
  }
  
  // скрутка массива БАЙТ
  byte troll[4] = {_L, _O, _L, _empty};
  disp.scrollByte(troll, 100);
  delay(500);
}

void twistClock() {
  byte hrs = 21, mins = 55;
  uint32_t tmr;
  Now = millis();
  while (millis () - Now < 5000) {   // каждые 10 секунд
    if (millis() - tmr > 500) {       // каждые полсекунды
      tmr = millis();
      flag = !flag;
      disp.point(flag);   // выкл/выкл точки

      if (flag) {
        // ***** часы! ****
        mins ++;
        if (mins > 59) {
          mins = 0;
          hrs++;
          if (hrs > 24) hrs = 0;
        }
        // ***** часы! ****
        byte dispTime[] = {(byte)hrs / 10, (byte)hrs % 10, (byte)mins / 10, (byte)mins % 10};
        disp.twist(dispTime, 35);    // выводим время массивом
      }
    }
  }
  disp.point(0);   // выкл точки
}

void scrolls() {
  disp.clear();
  delay(1000);
  for (byte i = 0; i < 10; i++) {
    disp.scroll(3, i, 50);
    delay(300);
  }

  // прокрутка массива БАЙТ
  byte troll[4] = {_t, _r, _o, _l};
  disp.scrollByte(troll, 100);
  delay(500);
}

void fadeBlink() {
  // пишем HELL
  disp.displayByte(_H, _E, _L, _L);

  Now = millis();
  while (millis () - Now < 3000) {    // 3 секунды
    for (int i = 7; i > 0; i--) {
      disp.brightness(i);   // меняем яркость
      delay(40);
    }
    for (int i = 0; i < 8; i++) {
      disp.brightness(i);   // меняем яркость
      delay(40);
    }
  }
}

void scrollClock() {
  byte hrs = 15, mins = 0;
  uint32_t tmr;
  Now = millis();
  while (millis () - Now < 5000) {   // каждые 10 секунд
    if (millis() - tmr > 500) {       // каждые полсекунды
      tmr = millis();
      flag = !flag;
      disp.point(flag);   // выкл/выкл точки

      if (flag) {
        // ***** часы! ****
        mins ++;
        if (mins > 59) {
          mins = 0;
          hrs++;
          if (hrs > 24) hrs = 0;
        }
        // ***** часы! ****
        byte dispTime[] = {(byte)hrs / 10, (byte)hrs % 10, (byte)mins / 10, (byte)mins % 10};
        disp.scroll(dispTime, 70);    // выводим время массивом
      }
    }
  }
  disp.point(0);   // выкл точки
}

void normClock() {
  byte hrs = 15, mins = 0;
  uint32_t tmr;
  Now = millis();
  while (millis () - Now < 5000) {   // 10 секунд
    if (millis() - tmr > 500) {       // каждые полсекунды
      tmr = millis();
      flag = !flag;
      disp.point(flag);   // выкл/выкл точки

      // ***** часы! ****
      mins ++;
      if (mins > 59) {
        mins = 0;
        hrs++;
        if (hrs > 24) hrs = 0;
      }
      // ***** часы! ****
      disp.displayClock(hrs, mins);   // выводим время функцией часов
    }
  }
  disp.point(0);   // выкл точки
}

void ints() {
  // тупо отправляем цифры
  disp.displayInt(-999);
  delay(200);
  disp.displayInt(-99);
  delay(200);
  disp.displayInt(-9);
  delay(200);
  disp.displayInt(0);
  delay(200);
  disp.displayInt(6);
  delay(200);
  disp.displayInt(66);
  delay(200);
  disp.displayInt(666);
  delay(200);
  disp.displayInt(6666);
  delay(200);
}

void runningText() {
  byte welcome_banner[] = {_H, _E, _L, _L, _O, _empty, _empty,
                           _T, _M, _1, _6, _3, _7,
                          };
  disp.runningString(welcome_banner, sizeof(welcome_banner), 150);  // 200 это время в миллисекундах!
}
