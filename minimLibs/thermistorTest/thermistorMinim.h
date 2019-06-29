// мини-класс для работы с термисторами по закону Стейнхарта-Харта
// GND --- термистор --- A0 --- 10к --- 5V

class thermistor {
  public:
    thermistor(byte pin, int resistance, int beta, int tempBase, byte resistBase);
    thermistor(byte pin, int resistance, int beta);
    float getTemp();
    float getTempAverage();
  private:
    float computeTemp(int analog);
    byte _pin;
    int _beta;
    int _tempBase = 25;
    int _resistance = 10000;
    byte _resistBase = 10000;
};

thermistor::thermistor(byte pin, int resistance, int beta, int tempBase, byte resistBase) {
  _pin = pin;
  _beta = beta;
  _tempBase = tempBase;
  _resistBase = resistBase;
  _resistance = resistance;
}

thermistor::thermistor(byte pin, int resistance, int beta) {
  _pin = pin;
  _beta = beta;
  _resistance = resistance;
}

float thermistor::computeTemp(int analog) {
  float temp;
  temp = _resistance / ((float)1023 / analog - 1);
  temp /= _resistBase;                        // (R/Ro)
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
  for (byte i = 0; i < 10; i++)
    analogAverage += analogRead(_pin);
  analogAverage /= 10;
  return computeTemp(analogAverage);
}
