#include "GyverMotor.h"
#include <Arduino.h>

GMotor::GMotor(uint8_t dig_pin, uint8_t pwm_pin) {
	_dig_pin = dig_pin;
	_pwm_pin = pwm_pin;
	
	pinMode(_dig_pin, OUTPUT);
	pinMode(_pwm_pin, OUTPUT);
	digitalWrite(_dig_pin, 0);
	digitalWrite(_pwm_pin, 0);
}

void GMotor::setDirection(uint8_t direction) {
	_reverse = direction;
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
	analogWrite(_pwm_pin, _duty);
}
void GMotor::runBkw10bit() {
	digitalWrite(_dig_pin, 1);
	analogWrite(_pwm_pin, 1023 - _duty);
}

void GMotor::setMode(uint8_t mode) {
	_mode = mode;
	if (mode == STOP) {
		digitalWrite(_dig_pin, 0);
		digitalWrite(_pwm_pin, 0);
	}
}

void PWM10bit() {
	// установка 9 и 10 пинов в режим 10 бит
	TCCR1A = TCCR1A & 0xe0 | 3;
}

void PWMfrequency(uint8_t pin, uint16_t mode) {
  byte prescale;
  if (pin == 5 || pin == 6) {
    switch (mode) {
      case 1: prescale = 0b001; break;
      case 2: prescale = 0b010; break;
      case 3: prescale = 0b011; break;
      case 4: prescale = 0b100; break;
      case 5: prescale = 0b101; break;
      default: return;
    }
  } else if (pin == 9 || pin == 10) {
	  switch (mode) {
      case 1: prescale = 0x09; break;
      case 2: prescale = 0x0a; break;
      case 3: prescale = 0x0b; break;
      case 4: prescale = 0x0c; break;
      case 5: prescale = 0x0d; break;
      default: return;
    }
  } else if (pin == 3 || pin == 11) {
    switch (mode) {
      case 1: prescale = 0b001; break;
      case 2: prescale = 0b010; break;
      case 3: prescale = 0b011; break;
      case 4: prescale = 0b100; break;
      case 5: prescale = 0b101; break;
      case 6: prescale = 0b110; break;
      case 7: prescale = 0b111; break;
      default: return;
    }
  }
  if (pin == 5 || pin == 6) {
    TCCR0B = TCCR0B & 0b11111000 | prescale;
  } else if (pin == 9 || pin == 10) {
    TCCR1B = TCCR1B & 0b11111000 | prescale;
  } else if (pin == 3 || pin == 11) {
    TCCR2B = TCCR2B & 0b11111000 | prescale;
  }
}