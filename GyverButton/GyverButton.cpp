#include "GyverButton.h"
#include <Arduino.h>

GButton::GButton(uint8_t pin) {
	_PIN = pin;
	GButton::init();
}
GButton::GButton(uint8_t pin, boolean type, boolean dir) {
	_PIN = pin;
	GButton::init();
	GButton::setType(type);
	_inv_state = dir;
}

void GButton::init() {
	_debounce = 100;
	_timeout = 500;
	_step_timeout = 400;
	_inv_state = NORM_OPEN;
	GButton::setType(HIGH_PULL);
}

void GButton::setDebounce(uint16_t debounce) {
	_debounce = debounce;
}
void GButton::setTimeout(uint16_t timeout) {
	_timeout = timeout;
}
void GButton::setStepTimeout(uint16_t step_timeout) {
	_step_timeout = step_timeout;
}
void GButton::setType(boolean type) {
	_type = type;
	if (type) pinMode(_PIN, INPUT);
	else pinMode(_PIN, INPUT_PULLUP);
}
void GButton::setDirection(boolean dir) {
	_inv_state = dir;
}
void GButton::setTickMode(boolean tickMode) {
	_tickMode = tickMode;
}

boolean GButton::isPress() {
	if (_tickMode) GButton::tick();
	if (flags.isPress_f) {
		flags.isPress_f = false;
		return true;
	} else return false;
}
boolean GButton::isRelease() {
	if (_tickMode) GButton::tick();
	if (flags.isRelease_f) {
		flags.isRelease_f = false;
		return true;
	} else return false;
}
boolean GButton::isClick() {	
	if (_tickMode) GButton::tick();
	if (flags.isOne_f) {
		flags.isOne_f = false;
		return true;
	} else return false;
}
boolean GButton::isHolded() {
	if (_tickMode) GButton::tick();
	if (flags.isHolded_f) {
		flags.isHolded_f = false;
		return true;
	} else return false;
}
boolean GButton::isHold() {
	if (_tickMode) GButton::tick();
	if (flags.step_flag) return true;
	else return false;
}
boolean GButton::state() {
	if (_tickMode) GButton::tick();
	return flags.btn_state;
}
boolean GButton::isSingle() {
	if (_tickMode) GButton::tick();
	if (flags.counter_flag && last_counter == 1) {
		flags.counter_flag = false;
		return true;
	} else return false;
}
boolean GButton::isDouble() {
	if (_tickMode) GButton::tick();
	if (flags.counter_flag && last_counter == 2) {
		flags.counter_flag = false;
		return true;
	} else return false;
}
boolean GButton::isTriple() {
	if (_tickMode) GButton::tick();
	if (flags.counter_flag && last_counter == 3) {
		flags.counter_flag = false;
		return true;
	} else return false;
}
boolean GButton::hasClicks() {
	if (_tickMode) GButton::tick();
	if (flags.counter_flag) {
		flags.counter_flag = false;
		return true;
	} else return false;
}
uint8_t GButton::getClicks() {
	return last_counter;	
}
boolean GButton::isStep() {
	if (_tickMode) GButton::tick();
	if (flags.step_flag && (millis() - btn_timer >= _step_timeout)) {
		btn_timer = millis();
		return true;
	}
	else return false;
}
void GButton::tick(boolean state) {
	_mode = true;
	flags.btn_state = state ^ _inv_state;
	GButton::tick();
	_mode = false;
}
void GButton::tick() {	
	if (!_mode) flags.btn_state = !digitalRead(_PIN) ^ (_inv_state ^ _type);
	
  if (flags.btn_state && !flags.btn_flag && (millis() - btn_timer >= _debounce)) {
    flags.btn_flag = true;
    btn_counter++;
    btn_timer = millis();
    flags.isPress_f = true;
	flags.oneClick_f = true;
  }
  if (!flags.btn_state && flags.btn_flag) {
    flags.btn_flag = false;
    flags.hold_flag = false;
    flags.isRelease_f = true;
	btn_timer = millis();
	flags.step_flag = false;
	if (flags.oneClick_f) {
		flags.oneClick_f = false;
		flags.isOne_f = true;
	}
  }
  if (flags.btn_flag && flags.btn_state && (millis() - btn_timer >= _timeout) && !flags.hold_flag) {
    flags.hold_flag = true;
    btn_counter = 0;
    flags.isHolded_f = true;
	flags.step_flag = true;
	flags.oneClick_f = false;
	btn_timer = millis();
  }
  
  if ((millis() - btn_timer >= _timeout) && (btn_counter != 0)) {    
    last_counter = btn_counter;
    btn_counter = 0;
	flags.counter_flag = true;
  }
}