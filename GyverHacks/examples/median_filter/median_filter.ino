#include "GyverHacks.h"

int val[3];   // массив для хранения трёх последних измерений
float val_f;
byte i;

void setup() {
  Serial.begin(9600);
}

void loop() {
  // счётчик от 0 до 2
  // каждую итерацию таймера i последовательно принимает значения 0, 1, 2, и так по кругу
  if (i > 1) i = 0;
  else i++;

  val[i] = analogRead(0);
  val_f = medianFilter(val[0], val[1], val[2]);    // фильтровать медианным фильтром из 3ёх последних измерений
  Serial.println(val_f);
  delay(100);

}
