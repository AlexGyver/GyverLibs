#ifndef thermistorMinim_h
#define thermistorMinim_h
// мини-класс для работы с термисторами по закону Стейнхарта-Харта
// GND --- термистор --- A0 --- 10к --- 5V
#define SAMPLE_AVERAGE 20   // количество чтений для усреднения

class thermistor {
public:
	thermistor(byte pin, int resistance, int beta, int tempBase = 25, int resistBase = 10000) :
	_pin(pin), _resistance(resistance), _beta(beta), _tempBase(tempBase), _resistBase(resistBase) {}
	
	float getTemp() {					// прочитать температуру с пина
		return computeTemp(analogRead(_pin));
	}
	
	float getTempAverage() {			// прочитать усреднённую температуру с пина
		int analogAverage = 0;
		for (byte i = 0; i < SAMPLE_AVERAGE; i++)
		analogAverage += analogRead(_pin);
		analogAverage /= (int)SAMPLE_AVERAGE;
		return computeTemp(analogAverage);
	}
	
	float computeTemp(int analog) {		// получить температуру из 10 бит сигнала АЦП
		float temp;
		temp = _resistBase / ((float)1024 / analog - 1);
		temp /= (float)_resistance;                 // (R/Ro)
		temp = log(temp) / _beta;            		// 1/B * ln(R/Ro)
		temp += (float)1.0 / (_tempBase + 273.15);  // + (1/To)
		temp = (float)1.0 / temp - 273.15;    		// инвертируем и конвертируем в градусы по Цельсию
		return temp;
	}
	
private:    
	const byte _pin = 0;
	const int _beta = 0;
	const int _tempBase = 25;
	const int _resistance = 10000;
	const int _resistBase = 10000;
};
#endif