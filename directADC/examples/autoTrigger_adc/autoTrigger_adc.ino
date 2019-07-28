#include <directADC.h>
void setup() {
  Serial.begin(9600);
  ADC_enable(); // вызывается обязательно
  ADC_setPrescaler(64); // без вызова - делитель 2
  ADC_setReference(ADC_VCC); // без вызова - ADC_AREF
  setAnalogMux(ADC_A4); // выбрать ADC_A0-ADC_A7 / ADC_SENSOR - термометр / ADC_1V1 / ADC_GND // Без вызова - ADC_A0
  ADC_autoTriggerEnable(FREE_RUN); // другие варианты см в файле directADC.h
  ADC_startConvert(); // пнуть ацп
}

void loop() {
  Serial.println(ADC_read()); // читаем актуальное значение в любой момент времени, без ожидания
  // можно заменить на ADC_readWhenAvailable() чтобы быть уверенным в том,что результат наиболее свежий (вносит задержку 10-100 мкс в код)
}
