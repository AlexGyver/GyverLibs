#pragma once
#include <math.h>

// альфа-бета фильтр
class GABfilter {
public:
	// период дискретизации (измерений), process variation, noise variation
	GABfilter(float delta, float sigma_process, float sigma_noise) {setParameters(delta, sigma_process, sigma_noise);}
	
	// период дискретизации (измерений), process variation, noise variation
	void setParameters(float delta, float sigma_process, float sigma_noise) {
		dt = delta;
		float lambda = (float)sigma_process * dt * dt / sigma_noise;
		float r = (4 + lambda - (float)sqrt(8 * lambda + lambda * lambda)) / 4;
		a = (float)1 - r * r;
		b = (float)2 * (2 - a) - 4 * (float)sqrt(1 - a);
	}
	
	// возвращает фильтрованное значение
	float filtered(float value) {				
		xm = value;
		xk = xk_1 + ((float) vk_1 * dt );
		vk = vk_1;
		rk = xm - xk;
		xk += (float)a * rk;
		vk += (float)( b * rk ) / dt;
		xk_1 = xk;
		vk_1 = vk;
		return xk_1;
	}
	
private:
	float dt;
	float xk_1, vk_1, a, b;
	float xk, vk, rk;
	float xm;
};