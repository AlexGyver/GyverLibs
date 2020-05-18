#include <directADC.h>
void setup() {
  Serial.begin(9600);
  ADC_enable();
  ADC_setPrescaler(64);
  ADC_setReference(ADC_VCC);
  setAnalogMux(ADC_A0);
  ADC_setResolution(8);
}

void loop() {
  ADC_startConvert();
  while(!ADC_available());
  Serial.println(ADC_read8());
  delay(10); 
}
