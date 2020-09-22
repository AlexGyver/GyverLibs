#pragma once
#include <Arduino.h>

// линейная аппроксимация методом наименьших квадратов
template < typename TYPE >
class GLinear {
public:
	GLinear(){};
	void compute(TYPE *x_array, TYPE *y_array, int arrSize) {		// аппроксимировать
		int32_t sumX = 0, sumY = 0, sumX2 = 0, sumXY = 0;	
		arrSize /= sizeof(int);
		for (int i = 0; i < arrSize; i++) {		// для всех элементов массива
			sumX += x_array[i];
			sumY += (long)y_array[i];
			sumX2 += x_array[i] * x_array[i];
			sumXY += (long)y_array[i] * x_array[i];
		}
		a = (long)arrSize * sumXY;             // расчёт коэффициента наклона приямой
		a = a - (long)sumX * sumY;
		a = (float)a / (arrSize * sumX2 - sumX * sumX);
		b = (float)(sumY - (float)a * sumX) / arrSize;
		delta = a * (x_array[arrSize-1] - x_array[0]);			// расчёт изменения
	}
	float getA() {return a;}			// получить коэффициент А
	float getB() {return b;}			// получить коэффициент В
	float getDelta() {return delta;}	// получить аппроксимированное изменение
	
private:	
	float a, b, delta;
};

/*
Сам алгоритм выглядит так:
void loop() {
  sumX = 0;
  sumY = 0;
  sumX2 = 0;
  sumXY = 0;
  for (int i = 0; i < steps; i++) {
    sumX += X[i];
    sumY += Y[i];
    sumX2 += X[i] * X[i];
    sumXY += X[i] * Y[i];
  }
  a = (steps * sumXY - sumX * sumY) / (steps * sumX2 - sumX * sumX);
  b = (sumY - a * sumX) / steps;
  int delta = steps * a;
}
*/