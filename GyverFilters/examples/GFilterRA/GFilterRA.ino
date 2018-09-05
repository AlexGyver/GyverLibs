#include "GyverFilters.h"
GFilterRA analog0;    // фильтр назовём analog0

void setup() {
  Serial.begin(9600);

  // установка коэффициента фильтрации (0.0... 1.0). Чем меньше, тем плавнее фильтр
  analog0.setCoef(0.01);

  // установка шага фильтрации (мс). Чем меньше, тем резче фильтр
  analog0.setStep(10);
}

void loop() {
  Serial.println(analog0.filteredTime(analogRead(0)));
}
