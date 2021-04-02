#pragma once
#include <Arduino.h>

// линейная аппроксимация методом наименьших квадратов
template < typename TYPE, int SIZE >
class RingAverage {
public:
	RingAverage() {
		for (int i = 0; i < SIZE; i++) buf[i] = 0;
	}
	TYPE filtered(TYPE val) {
		if (++t >= SIZE) t = 0; 	// перемотка t
		sum -= buf[t];     			// вычитаем старое
		sum += val;         		// прибавляем новое
		buf[t] = val;          		// запоминаем в массив
		return (sum / SIZE);
	}
	float filteredFloat(TYPE val) {
		if (++t >= SIZE) t = 0; 	// перемотка t
		sum -= buf[t];     			// вычитаем старое
		sum += val;         		// прибавляем новое
		buf[t] = val;          		// запоминаем в массив
		return ((float)sum / SIZE);
	}
	
private:	
	TYPE buf[SIZE];
	int32_t sum = 0;
	int t = 0;
};