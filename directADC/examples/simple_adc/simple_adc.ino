#include <directADC.h>
/* пример простой работы с ацп */
void setup() {
  Serial.begin(9600);
  ADC_enable(); // вызывается обязательно
  ADC_setPrescaler(64); // без вызова - делитель 2
  ADC_setReference(ADC_VCC); // без вызова - ADC_AREF
  setAnalogMux(ADC_A4); // выбрать ADC_A0-ADC_A7 / ADC_SENSOR - термометр / ADC_1V1 / ADC_GND // Без вызова - ADC_A0
}

void loop() {
  ADC_startConvert(); // ручной старт преобразования
  while (!ADC_available()); // пока преобразование не готово - ждем или делаем что то полезное
  Serial.println(ADC_read());

  // ADC_read(); - прямая склейка и чтение,если преобразование еще не закончилось - вернет ошибочное значение.
  // ADC_readWhenAvailable(); - дождется окончания преобразования и вернет результат, если уже готово-вернет сразу.
}
