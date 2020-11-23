#pragma once
// мини-класс для работы с термисторами по закону Стейнхарта-Харта
// GND --- термистор --- A0 --- 10к --- 5V
#define SAMPLE_AVERAGE 20   // количество чтений для усреднения

class thermistor {
  public:
    thermistor(byte pin, int resistance, int beta, int tempBase = 25, int resistBase = 10000);
    float getTemp();
    float getTempAverage();
  private:
    float computeTemp(int analog);
    byte _pin = 0;
    int _beta = 0;
    int _tempBase = 25;
    int _resistance = 10000;
    int _resistBase = 10000;
};

thermistor::thermistor(byte pin, int resistance, int beta, int tempBase, int resistBase) {
  _pin = pin;
  _beta = beta;
  _tempBase = tempBase;
  _resistBase = resistBase;
  _resistance = resistance;
}

float thermistor::computeTemp(int analog) {
  float temp;
  temp = _resistBase / ((float)1024 / analog - 1);
  temp /= (float)_resistance;                        // (R/Ro)
  temp = log(temp) / _beta;            		// 1/B * ln(R/Ro)
  temp += (float)1.0 / (_tempBase + 273.15);  // + (1/To)
  temp = (float)1.0 / temp - 273.15;    		// инвертируем и конвертируем в градусы по Цельсию
  return temp;
}

float thermistor::getTemp() {
  return computeTemp(analogRead(_pin));
}

float thermistor::getTempAverage() {
  int analogAverage = 0;
  for (byte i = 0; i < SAMPLE_AVERAGE; i++)
    analogAverage += analogRead(_pin);
  analogAverage /= SAMPLE_AVERAGE;
  return computeTemp(analogAverage);
}