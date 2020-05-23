#pragma once
#include <math.h>

// упрощённый Калман для одномерного случая
class GKalman {
public:
	// разброс измерения, разброс оценки, скорость изменения значений
	GKalman(float mea_e, float est_e, float q) { setParameters(mea_e, est_e, q); }
	
	// разброс измерения, скорость изменения значений (разброс измерения принимается равным разбросу оценки)
	GKalman(float mea_e, float q) {GKalman::setParameters(mea_e, mea_e, q);}
	
	// разброс измерения, разброс оценки, скорость изменения значений
	void setParameters(float mea_e, float est_e, float q) {
		_err_measure = mea_e;
		_err_estimate = est_e;
		_q = q;
	}
	
	// разброс измерения, скорость изменения значений (разброс измерения принимается равным разбросу оценки)
	void setParameters(float mea_e, float q) {setParameters(mea_e, mea_e, q);}
	
	// возвращает фильтрованное значение
	float filtered(float value) {		
		float _kalman_gain, _current_estimate;
		_kalman_gain = _err_estimate / (_err_estimate + _err_measure);
		_current_estimate = _last_estimate + _kalman_gain * (value - _last_estimate);
		_err_estimate =  (1.0 - _kalman_gain)*_err_estimate + fabs(_last_estimate-_current_estimate)*_q;
		_last_estimate=_current_estimate;
		return _current_estimate;
	}

private:
	float _err_measure = 0.0;
	float _err_estimate = 0.0;
	float _q = 0.0;
	float _last_estimate = 0.0;
};