// пример с плавным миганием светодиода

#include "CRT.h"

void setup() {
  pinMode(13, OUTPUT);
}

void loop() {
  static int val = 0;
  // шим на 13 пине для теста
  //softPWM(13, val);               // голое значение
  softPWM(13, CRT8_table(val));     // через CRT

  // алгоритм плавного мигания
  static uint32_t tmr;
  if (millis() - tmr >= 5) {
    tmr = millis();    
    static int8_t dir = 1;
    val += dir;    
    if (val == 255 || val == 0) dir = -dir;   // разворачиваем
  }  
}

// софт шим
void softPWM(byte pin, byte val) {
  static byte count;
  count++;
  if (count == 0) digitalWrite(pin, 1);
  if (count == val) digitalWrite(pin, 0);
}
