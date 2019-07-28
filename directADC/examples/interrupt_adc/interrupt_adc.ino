#include <directADC.h>
void setup() {
  Serial.begin(9600);
  ADC_enable(); // вызывается обязательно
  ADC_setPrescaler(64); // без вызова - делитель 2
  ADC_setReference(ADC_VCC); // без вызова - ADC_AREF
  ADC_attachInterrupt(adcReady); // добавим прерывание готовности
  setAnalogMux(ADC_A4); // выбрать ADC_A0-ADC_A7 / ADC_SENSOR - термометр / ADC_1V1 / ADC_GND // Без вызова - ADC_A0
}


void loop() {
  delay(1);
  ADC_startConvert(); // начинаем преобразование
}

void adcReady() {
  Serial.println(ADC_read()); // забираем результат
}
