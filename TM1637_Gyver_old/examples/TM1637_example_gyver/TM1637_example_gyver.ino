/*
   Пример вывода на дисплей с регистром TM1637
   AlexGyver Technologies http://alexgyver.ru/
*/

//--- ПИНЫ ---
#define CLK 5
#define DIO 4
//--- ПИНЫ ---

//--- ОБЪЯВЛЕНИЕ ---
#include "TM1637.h"
TM1637 disp(CLK, DIO);
//--- ОБЪЯВЛЕНИЕ ---

void setup() {
  Serial.begin(9600);
  disp.init();  // инициализация
  disp.set(7);  // яркость, 0 - 7 (минимум - максимум)
}

void loop() {
  // забиваем массив буквами из библиотеки
  byte welcome_banner[] = {_H, _E, _L, _L, _O, _empty, _empty,
                           _e, _n, _j, _o, _y, _empty, _empty,
                           _1, _6, _3, _7, _empty, _D, _i, _S, _P, _l, _a, _y
                          };
  // выводим на дисплей со смещением на один шаг
  Serial.println(sizeof(welcome_banner));
  disp.runningString(welcome_banner, sizeof(welcome_banner), 300);
  delay(700);
  disp.clearDisplay();

  disp.displayByte(0x76, 0x79, 0x38, 0x38);  // вывести HELL "вручную"
  delay(2000);
  disp.clearDisplay();

  disp.displayByte(_H, _E, _L, _L);   // вывести HELL, используя буквы из библиотеки
  delay(2000);
  disp.clearDisplay();

  byte lol[] = {1, 2, 3, 4};    // вывести 1234 из массива
  disp.display(lol);
  delay(2000);
  disp.clearDisplay();

  for (byte j = 0; j < 99; j++) {  // вывести 0-99 счётчик с нулями слева
    disp.displayIntZero(j);
    delay(20);
  }
  disp.clearDisplay();

  for (int j = 0; j < 1100; j += 5) {  // счётчик 0-1100 без нулей
    disp.displayInt(j);
    delay(20);
  }
  disp.clearDisplay();

  for (byte i = 0; i <= 3; i++) {       // выводим цифры 0-9 по очереди в каждый порт
    for (byte j = 0; j <= 9; j++) {
      disp.display(i, j);
      delay(100);
    }
    disp.clearDisplay();
  }
  disp.clearDisplay();
}

/*
  // нумерация портов слева направо!
  disp.display(port, num);      // вывести цифру num (0-9) в порт port (0-3)
  disp.display(ARRAY);          // вывести ARRAY, где ARRAY - массив из 4х цифр ( byte ARRAY[4] = {0, 1, 2, 3}; )
  disp.displayByte(port, Byte); // вывести символ Byte в порт port (0-3) (Byte вида 0x7d)
  disp.displayByte(ARRAY_BYTE); // ARRAY_BYTE - массив кодов ( 0x7d, 0x7d, 0x7d, 0x7d )
  disp.displayByte(Byte1, Byte2, Byte3, Byte4); // вывести побайтово
  // для вывода некоторых букв доступен вывод вида _A, _b, _h. Полный список смотри в TM1637.h
  disp.displayInt(value);       // вывести 1-4 значное число
  disp.displayIntZero(value);   // вывести 1-4 значное число с нулями СЛЕВА

  // бегущая строка символов из массива arrayBytes, число символов sizeof(arrayBytes), интервал delay_time в миллисекундах
  disp.runningString(arrayBytes, sizeof(arrayBytes), delay_time); 

  disp.point(state);        // включить/выключить двоеточие (1 и 0 соответственно)

  disp.clearDisplay();      // очистить дисплей
  disp.set(7);              // яркость, 0 - 7 (минимум - максимум)
*/



