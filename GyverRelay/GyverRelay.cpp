#include "GyverRelay.h"
#include <Arduino.h>

GyverRelay::GyverRelay(boolean direction) {
	_direction = direction;
	output = !_direction;   // выключить реле сразу
}

void GyverRelay::setDirection(boolean dir) {
	_direction = dir;
}

int signum(float val) {
	return ((val > 0) ? 1 : ((val < 0) ? -1 : 0));
}

// вернёт выход, принимает время итерации в секундах
boolean GyverRelay::compute(float dt) {
	float signal;
	if (dt > 0) {
		float rate = (input - prevInput) / dt;    // производная от величины (величина/секунду)
		prevInput = input;
		signal = input + rate * k;
	} else {
		signal = input;
	}

	// жуткая функция реле из лекций по ТАУ
	int8_t F = (signum(signal - setpoint - hysteresis / 2) + signum(signal - setpoint + hysteresis / 2)) / 2;

	if (F == 1) output = !_direction;
	else if (F == -1) output = _direction;
	return output;
}

boolean GyverRelay::getResult() {
	GyverRelay::compute((millis() - prevTime) / 1000.0f);
	prevTime = millis();
	return output;
}

boolean GyverRelay::getResultTimer() {
	if (millis() - prevTime > dT) {
		prevTime = millis();
		GyverRelay::compute((float)dT / 1000);
	}
	return output;
}