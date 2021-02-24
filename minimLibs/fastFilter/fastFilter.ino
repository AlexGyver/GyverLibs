// быстрый запаздывающйи фильтр

#include "FastFilter.h"
FastFilter fil(29);   // 0-32
void setup() {
  Serial.begin(9600);
  fil.setRaw(1000);
  fil.setFil(0);
}

void loop() {
  fil.compute();
  Serial.println(fil.getFil());
  delay(100);
}
