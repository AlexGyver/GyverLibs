#include "GyverRelay.h"
#include <Arduino.h>

GyverRelay::GyverRelay() {}

GyverRelay::GyverRelay(float new_setpoint, float new_hysteresis, modes direction) {
	setpoint = new_setpoint;
	hysteresis = new_hysteresis;
	_direction = direction;
	_relayState = !_direction;		// выключить реле сразу
}

void GyverRelay::setDirection(modes direction) {
	_direction = direction;
}

int8_t sign(float value) {
	if (value > 0) return 1;
	else if (value < 0) return -1;
}

boolean GyverRelay::compute(float dt) {
	rate = (float)(input - prevInput) / dt;		// производная от величины (величина/секунду)
	prevInput = input;
	signal = input + rate * k;
	
	// жуткая функция реле из лекций
	int8_t F = ((float)sign(signal - setpoint - hysteresis / 2) + (float)sign(signal - setpoint + hysteresis / 2)) / 2;
	
	if (F == 1) _relayState = !_direction;
	else if (F == -1) _relayState = _direction;
}

boolean GyverRelay::getResult() {
	GyverRelay::compute((float)(millis() - prevTime) / 1000);	
	prevTime = millis();
	return _relayState;
}
boolean GyverRelay::getResultTimer() {
	if ((long)millis() > prevTime) {
		prevTime = millis() + sampleTime;		
		GyverRelay::compute((float)sampleTime / 1000);
		return _relayState;
	} else {
		return _relayState;
	}
}

boolean GyverRelay::getResult(float new_input) {
	input = new_input;
	GyverRelay::getResult();
	return _relayState;
}
boolean GyverRelay::getResultTimer(float new_input) {
	input = new_input;
	GyverRelay::getResultTimer();
	return _relayState;
}