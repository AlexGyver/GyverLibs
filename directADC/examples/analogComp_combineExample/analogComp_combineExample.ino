/* данный скетч инициирует запуск преобразования ацп при снижении напряжения на входе ниже определенного порога */
#include <directADC.h>
void setup() {
  Serial.begin(9600);

  setAnalogMux(ADC_A0); // настроим вход АЦП

  ADC_enable(); // включаем ацп
  ADC_setPrescaler(64); // настраиваем делитель
  ADC_setReference(ADC_VCC); // опорное 5В
  ADC_attachInterrupt(adcReady); // заберем значение ацп как только оно будет готово
  ADC_autoTriggerEnable(ANALOG_COMP); // ацп запустится от срабатывания компаратора (RISING)

  ACOMP_attachInterrupt(ivent, RISING_TRIGGER);
  ACOMP_setPositiveInput(ADC_1V1); //порог срабатывания - 1.1В
  // второй вывод по дефолту висит на pin 7 , поэтому наблюдаемое напряжение повесим на pin 7
}

void ivent() {}; // пустая функция прерывания компаратора

void adcReady() {
  Serial.println(ADC_read());
}
// Внимание! Когда напряжения на входах компаратора будут близки, с высокой вероятностью начнет проявляться дребезг */
void loop() {
}
