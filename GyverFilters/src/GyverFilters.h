#pragma once
#include <filters/alfaBeta.h>
#include <filters/kalman.h>
#include <filters/linear.h>
#include <filters/median.h>
#include <filters/median3.h>
#include <filters/runningAverage.h>

/*
	GyverFilters - библиотека с некоторыми удобными фильтрами:
	- GFilterRA - компактная альтернатива фильтра экспоненциальное бегущее среднее (Running Average)			
	- GMedian3 - быстрый медианный фильтр 3-го порядка (отсекает выбросы)
	- GMedian - медианный фильтр N-го порядка. Порядок настраивается в GyverFilters.h - MEDIAN_FILTER_SIZE
	- GABfilter - альфа-бета фильтр (разновидность Калмана для одномерного случая)
	- GKalman - упрощённый Калман для одномерного случая (на мой взгляд лучший из фильтров)
	- GLinear - линейная аппроксимация методом наименьших квадратов для двух массивов
	
	Версии
	- 1.6 от 12.11.2019
	- 1.7: исправлен GLinear
	- 1.8: небольшие улучшения
	- 2.0:
		- Улучшен и исправлен median и median3
		- Улучшен linear
		- Смотрите примеры! Использование этих фильтров чуть изменилось
*/