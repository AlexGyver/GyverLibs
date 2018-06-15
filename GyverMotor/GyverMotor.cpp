#include "GyverMotor.h"
#include <Arduino.h>
#include <TimerOne.h>

GMotor::GMotor(uint8_t dig_pin, uint8_t pwm_pin) {
	_dig_pin = dig_pin;
	_pwm_pin = pwm_pin;
}

void GMotor::init() {
	pinMode(_dig_pin, OUTPUT);
	pinMode(_pwm_pin, OUTPUT);
	digitalWrite(_dig_pin, 0);
	digitalWrite(_pwm_pin, 0);
}

void GMotor::init10bit(uint32_t freq) {
	pinMode(_dig_pin, OUTPUT);
	pinMode(_pwm_pin, OUTPUT);
	digitalWrite(_dig_pin, 0);
	digitalWrite(_pwm_pin, 0);
	
	uint32_t period = 1000000 / freq;
	Timer1.initialize(period);
}

void GMotor::reverse(boolean reverse) {
	_reverse = reverse;
}

void GMotor::setSpeed(uint8_t duty) {
	_duty = duty;
	if (_mode == FORWARD)
		if (_reverse) runFrw();
		else runBkw();
	else if (_mode == BACKWARD)
		if (_reverse) runBkw();
		else runFrw();
}

void GMotor::setSpeed10bit(uint16_t duty) {
	_duty = duty;
	if (_mode == FORWARD)
		if (_reverse) runFrw10bit();
		else runBkw10bit();
	else if (_mode == BACKWARD)
		if (_reverse) runBkw10bit();
		else runFrw10bit();
}

void GMotor::runFrw() {
	digitalWrite(_dig_pin, 0);
	analogWrite(_pwm_pin, _duty);
}
void GMotor::runBkw() {
	digitalWrite(_dig_pin, 1);
	analogWrite(_pwm_pin, 255 - _duty);
}

void GMotor::runFrw10bit() {
	digitalWrite(_dig_pin, 0);
	Timer1.pwm(_pwm_pin, _duty);
}
void GMotor::runBkw10bit() {
	digitalWrite(_dig_pin, 1);
	Timer1.pwm(_pwm_pin, 1023 - _duty);
}

void GMotor::setMode(uint8_t mode) {
	_mode = mode;
	if (mode == STOP) {
		digitalWrite(_dig_pin, 0);
		digitalWrite(_pwm_pin, 0);
	}
}