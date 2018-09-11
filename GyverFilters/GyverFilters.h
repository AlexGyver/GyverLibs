#ifndef GyverFilters_h
#define GyverFilters_h
#include <Arduino.h>
#define LIBRARY_VERSION	1.2

#define	MEDIAN_FILTER_SIZE	(8)		// размер медианного фильтра!

/*
Текущая версия: 1.2 от 12.09.2018
	GyverFilters - библиотека с некоторыми удобными фильтрами:
		GFilterRA - компактная альтернатива фильтра бегущее среднее (Running Average)
			Класс GFilterRA
			setCoef(coef) - настройка коэффициента фильтрации (0.00 - 1.00)
			setStep (step) - настройка шага дискретизации (период фильтрации) - встроенный таймер! миллисекунды
			filtered(value) - возвращает фильтрованное значение
			filteredTime(value) - возвращает фильтрованное с опорой на встроенный таймер			
		GMedian3 - быстрый медианный фильтр 3-го порядка (отсекает выбросы)
			Класс GMedian3
			filtered(value) - выводит медиану из трёх последних обращений (там свой встроенный массив)
		GMedian - медианный фильтр N-го порядка (отсекает выбросы). Порядок настраивается в GyverFilters.h - MEDIAN_FILTER_SIZE
			Класс GMedian
			filtered(value) - выводит медиану из N последних обращений (там свой встроенный массив)
		GABfilter - альфа-бета фильтр (разновидность Калмана для одномерного случая)
			Класс GABfilter(delta, sigma_process, sigma_noise) - период дискретизации (измерений), process variation, noise variation
			setParameters(delta, sigma_process, sigma_noise) - перенастроить параметры
			filtered(value) - возвращает фильтрованное значение
		GKalman - упрощённый Калман для одномерного случая (на мой взгляд лучший из фильтров)
			Класс GKalman(разброс измерения, разброс оценки, скорость изменения значений) - читайте пример
			setParameters(разброс измерения, разброс оценки, скорость изменения значений) - перенастроить параметры
			filtered(value) - возвращает фильтрованное значение			
*/

class GFilterRA
{
  public:
	GFilterRA();
	GFilterRA(float, uint16_t);		// расширенная инициализация фильтра (коэффициент, шаг фильтрации)
	void setCoef(float);	    	// настройка коэффициента фильтрации (0.00 - 1.00). Чем меньше, тем плавнее
	void setStep(uint16_t);			// установка шага фильтрации (мс). Чем меньше, тем резче фильтр
	float filteredTime(int16_t);	// возвращает фильтрованное с опорой на встроенный таймер	
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