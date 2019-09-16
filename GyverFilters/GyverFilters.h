#ifndef GyverFilters_h
#define GyverFilters_h
#include <Arduino.h>
#define LIBRARY_VERSION	1.4

const byte MEDIAN_FILTER_SIZE = 8;		// размер медианного фильтра!

/*
	Текущая версия: 1.5 от 27.07.2019
	GyverFilters - библиотека с некоторыми удобными фильтрами:
	- GFilterRA - компактная альтернатива фильтра экспоненциальное бегущее среднее (Running Average)			
	- GMedian3 - быстрый медианный фильтр 3-го порядка (отсекает выбросы)
	- GMedian - медианный фильтр N-го порядка. Порядок настраивается в GyverFilters.h - MEDIAN_FILTER_SIZE
	- GABfilter - альфа-бета фильтр (разновидность Калмана для одномерного случая)
	- GKalman - упрощённый Калман для одномерного случая (на мой взгляд лучший из фильтров)
	- GLinear - линейная аппроксимация методом наименьших квадратов для двух массивов
*/

// компактная альтернатива фильтра экспоненциальное бегущее среднее
class GFilterRA
{
  public:
	GFilterRA();								// инициализация фильтра
	GFilterRA(float coef);						// расширенная инициализация фильтра (коэффициент)
	GFilterRA(float coef, uint16_t interval);	// расширенная инициализация фильтра (коэффициент, шаг фильтрации)
	void setCoef(float coef);	    			// настройка коэффициента фильтрации (0.00 - 1.00). Чем меньше, тем плавнее
	void setStep(uint16_t interval);			// установка шага фильтрации (мс). Чем меньше, тем резче фильтр
	
	float filteredTime(int16_t value);			// возвращает фильтрованное значение с опорой на встроенный таймер	
	float filtered(int16_t value);				// возвращает фильтрованное значение
	
	float filteredTime(float value);			// возвращает фильтрованное значение с опорой на встроенный таймер	
	float filtered(float value);				// возвращает фильтрованное значение
	
  private:
	float _coef = 0.0, _lastValue = 0.0;
	uint32_t _filterTimer = 0;
	uint16_t _filterInterval = 0;
};

// быстрый медианный фильтр 3-го порядка
class GMedian3
{
	public:
		GMedian3();
		uint16_t filtered(uint16_t value);	// возвращает фильтрованное значение
		
	private:
		uint16_t buffer[3];
		byte counter = 0;
};

// медианный фильтр N-го порядка
class GMedian
{
	public:
		GMedian();
		uint16_t filtered(uint16_t value);	// возвращает фильтрованное значение
};

// альфа-бета фильтр
class GABfilter
{
	public:
		GABfilter(float delta, float sigma_process, float sigma_noise);
		// период дискретизации (измерений), process variation, noise variation
		
		void setParameters(float delta, float sigma_process, float sigma_noise);
		// период дискретизации (измерений), process variation, noise variation
		
		float filtered(float value);				// возвращает фильтрованное значение
		
	private:
		float dt;
		float xk_1, vk_1, a, b;
		float xk, vk, rk;
		float xm;
};

// упрощённый Калман для одномерного случая
class GKalman
{
	public:
		GKalman(float mea_e, float est_e, float q);
		// разброс измерения, разброс оценки, скорость изменения значений
		
		GKalman(float mea_e, float q);
		// разброс измерения, скорость изменения значений (разброс измерения принимается равным разбросу оценки)
		
		void setParameters(float mea_e, float est_e, float q);
		// разброс измерения, разброс оценки, скорость изменения значений
		
		void setParameters(float mea_e, float q);
		// разброс измерения, скорость изменения значений (разброс измерения принимается равным разбросу оценки)
		
		float filtered(float value);		// возвращает фильтрованное значение
  
	private:
		float _err_measure = 0.0;
		float _err_estimate = 0.0;
		float _q = 0.0;
		float _current_estimate = 0.0;
		float _last_estimate = 0.0;
		float _kalman_gain = 0.0;
};

// линейная аппроксимация методом наименьших квадратов
class GLinear {
	public:
		GLinear();
		void compute(int *x_array, int *y_array, int arrSize);		// аппроксимировать
		float getA();		// получить коэффициент А
		float getB();		// получить коэффициент В
		float getDelta();	// получить аппроксимированное изменение
	private:
		int32_t sumX, sumY, sumX2, sumXY;
		float a, b, delta;
};

#endif