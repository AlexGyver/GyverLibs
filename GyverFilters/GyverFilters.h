#ifndef GyverFilters_h
#define GyverFilters_h
#include <Arduino.h>
#define LIBRARY_VERSION	1.3

#define	MEDIAN_FILTER_SIZE	(8)		// размер медианного фильтра!

/*
	Текущая версия: 1.3 от 24.09.2018
	GyverFilters - библиотека с некоторыми удобными фильтрами:
	- GFilterRA - компактная альтернатива фильтра экспоненциальное бегущее среднее (Running Average)			
	- GMedian3 - быстрый медианный фильтр 3-го порядка (отсекает выбросы)
	- GMedian - медианный фильтр N-го порядка. Порядок настраивается в GyverFilters.h - MEDIAN_FILTER_SIZE
	- GABfilter - альфа-бета фильтр (разновидность Калмана для одномерного случая)
	- GKalman - упрощённый Калман для одномерного случая (на мой взгляд лучший из фильтров)
*/

class GFilterRA
{
  public:
	GFilterRA();					// инициализация фильтра
	GFilterRA(float, uint16_t);		// расширенная инициализация фильтра (коэффициент, шаг фильтрации)
	void setCoef(float);	    	// настройка коэффициента фильтрации (0.00 - 1.00). Чем меньше, тем плавнее
	void setStep(uint16_t);			// установка шага фильтрации (мс). Чем меньше, тем резче фильтр
	float filteredTime(int16_t);	// возвращает фильтрованное значение с опорой на встроенный таймер	
	float filtered(int16_t);		// возвращает фильтрованное значение
	
  private:
	float _coef, _lastValue;
	uint32_t _filterTimer;
	uint16_t _filterInterval;
};

class GMedian3
{
	public:
		GMedian3();
		uint16_t filtered(uint16_t);	// возвращает фильтрованное значение
		
	private:
		uint16_t buffer[3];
		byte counter;
};

class GMedian
{
	public:
		GMedian();
		uint16_t filtered(uint16_t);	// возвращает фильтрованное значение
};

class GABfilter
{
	public:
		GABfilter(float, float, float);				// период дискретизации (измерений), process variation, noise variation
		void setParameters(float, float, float);	// период дискретизации (измерений), process variation, noise variation
		float filtered(float value);				// возвращает фильтрованное значение
		
	private:
		float dt;
		float xk_1, vk_1, a, b;
		float xk, vk, rk;
		float xm;
};

class GKalman
{
	public:
		GKalman(float, float, float);				// разброс измерения, разброс оценки, скорость изменения значений
		GKalman(float, float);						// разброс измерения, скорость изменения значений (разброс измерения принимается равным разбросу оценки)
		void setParameters(float, float, float);	// разброс измерения, разброс оценки, скорость изменения значений
		void setParameters(float, float);			// разброс измерения, скорость изменения значений (разброс измерения принимается равным разбросу оценки)
		float filtered(float);						// возвращает фильтрованное значение
  
	private:
		float _err_measure;
		float _err_estimate;
		float _q;
		float _current_estimate;
		float _last_estimate;
		float _kalman_gain;
};

#endif