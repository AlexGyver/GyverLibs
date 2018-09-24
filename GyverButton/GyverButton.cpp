#include "GyverButton.h"
#include <Arduino.h>

GButton::GButton(uint8_t PIN) {
  _PIN = PIN;  
  _debounce = 80;
  _timeout = 500;
  _step_timeout = 400;
  _inv_state = NORM_OPEN;
  GButton::setType(HIGH_PULL);
}
GButton::GButton(uint8_t PIN, boolean type, boolean dir) {
	GButton::GButton(PIN);
	GButton::setType(type);
	_inv_state = dir;
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
	if (isPress_f) {
		isPress_f = false;
		return true;
	} else return false;
}
boolean GButton::isRelease() {
	if (_tickMode) GButton::tick();
	if (isRelease_f) {
		isRelease_f = false;
		return true;
	} else return false;
}
boolean GButton::isClick() {	
	if (_tickMode) GButton::tick();
	if (isOne_f) {
		isOne_f = false;
		return true;
	} else return false;
}
boolean GButton::isHolded() {
	if (_tickMode) GButton::tick();
	if (isHolded_f) {
		isHolded_f = false;
		return true;
	} else return false;
}
boolean GButton::isHold() {
	if (_tickMode) GButton::tick();
	if (step_flag) return true;
	else return false;
}
boolean GButton::state() {
	if (_tickMode) GButton::tick();
	return btn_state;
}
boolean GButton::isSingle() {
	if (_tickMode) GButton::tick();
	if (counter_flag && last_counter == 1) {
		counter_flag = false;
		return true;
	} else return false;
}
boolean GButton::isDouble() {
	if (_tickMode) GButton::tick();
	if (counter_flag && last_counter == 2) {
		counter_flag = false;
		return true;
	} else return false;
}
boolean GButton::isTriple() {
	if (_tickMode) GButton::tick();
	if (counter_flag && last_counter == 3) {
		counter_flag = false;
		return true;
	} else return false;
}
boolean GButton::hasClicks() {
	if (_tickMode) GButton::tick();
	if (counter_flag) {
		counter_flag = false;
		return true;
	} else return false;
}
uint8_t GButton::getClicks() {
	return last_counter;	
}
boolean GButton::isStep() {
	if (_tickMode) GButton::tick();
	if (step_flag && (millis() - btn_timer > _step_timeout)) {
		btn_timer = millis();
		return true;
	}
	else return false;
}
void GButton::tick(boolean state) {
	_mode = true;
	btn_state = state ^ _inv_state;
	GButton::tick();
	_mode = false;
}
void GButton::tick() {	
	if (!_mode) btn_state = !digitalRead(_PIN) ^ (_inv_state ^ _type);
	
  if (btn_state && !btn_flag && (millis() - btn_timer > _debounce)) {
    btn_flag = true;
    btn_counter++;
    btn_timer = millis();
    isPress_f = true;
	oneClick_f = true;
  }
  if (!btn_state && btn_flag) {
    btn_flag = false;
    hold_flag = false;
    isRelease_f = true;
	btn_timer = millis();
	step_flag = false;
	if (oneClick_f) {
		oneClick_f = false;
		isOne_f = true;
	}
  }
  if (btn_flag && btn_state && (millis() - btn_timer > _timeout) && !hold_flag) {
    hold_flag = true;
    btn_counter = 0;
    isHolded_f = true;
	step_flag = true;
	oneClick_f = false;
	btn_timer = millis();
  }
  
  if ((millis() - btn_timer > _timeout) && (btn_counter != 0)) {    
    last_counter = btn_counter;
    btn_counter = 0;
	counter_flag = true;
  }
}