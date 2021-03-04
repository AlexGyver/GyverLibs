#include "fixed.h"

void setup() {
  Serial.begin(9600);
  
  volatile fixed value(3, 100);    // 0.03
  value *= 100;           // 3.0 
  value /= 2;             // 1.5
  value += toFix(3, 10);  // 1.8 (+0.3) 
  Serial.println(value.toFloat());
  // 1730/188
  
  /*
  volatile float value = 0.03;
  value *= 100;           // 3.0 
  value /= 2;             // 1.5
  value += 0.3;           // 1.8 (+0.3) 
  Serial.println(value);
  // 3046/200
  */
  
  //Serial.println((int32_t&)value);  // вывод fixed числа
  //Serial.println(value.fix);        // вывод fixed числа
  //Serial.println(value.toInt());      // перевод в int
  //Serial.println(value.toFloat());  // перевод в float
  //Serial.println(expandFix(value)); // раскрывающий макрос
}

void loop() {

}
