// быстрый запаздывающйи фильтр

#include <GyverFilters.h>
FastFilter fil(29);   // 0-32
void setup() {
  Serial.begin(9600);
  fil.setCoef(30);
  fil.setRaw(1000);
  fil.setFil(0);
}

void loop() {
  fil.computeNow();
  Serial.println(fil.getFil());
  delay(100);
}
