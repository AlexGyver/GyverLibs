#include "GyverButton.h"
#include <Arduino.h>

GButton::GButton(uint8_t BUTT) {
  _BUTT = BUTT;  
  _debounce = 80;
  _timeout = 500;
  _step_timeout = 400;
  pinMode(_BUTT, INPUT_PULLUP);
}

void GButton::setDebounce(uint8_t debounce) {
	_debounce = debounce;
}
void GButton::setTimeout(uint16_t timeout) {
	_timeout = timeout;
}
void GButton::setStepTimeout(uint16_t step_timeout) {
	_step_timeout = step_timeout;
}

void GButton::inverse(boolean inv_state) {
	_inv_state = inv_state;
	if (inv_state) pinMode(_BUTT, INPUT);
}

boolean GButton::isPress() {
	if (isPress_f) {
		isPress_f = false;
		return true;
	} else return false;
}
boolean GButton::isRelease() {
	if (isRelease_f) {
		isRelease_f = false;
		return true;
	} else return false;
}
boolean GButton::isClick() {	
	if (isOne_f) {
		isOne_f = false;
		return true;
	} else return false;
}
boolean GButton::isHolded() {
	if (isHolded_f) {
		isHolded_f = false;
		return true;
	} else return false;
}
boolean GButton::state() {
	if (_state) return true;
	else return false;
}
boolean GButton::isSingle() {
	if (counter_flag && last_counter == 1) {
		counter_flag = false;
		return true;
	} else return false;
}
boolean GButton::isDouble() {
	if (counter_flag && last_counter == 2) {
		counter_flag = false;
		return true;
	} else return false;
}
boolean GButton::isTriple() {
	if (counter_flag && last_counter == 3) {
		counter_flag = false;
		return true;
	} else return false;
}
boolean GButton::hasClicks() {
	if (counter_flag) {
		counter_flag = false;
		return true;
	} else return false;
}
uint8_t GButton::getClicks() {
	return last_counter;	
}

boolean GButton::isStep() {
	if (step_flag && (millis() - btn_timer > _step_timeout)) {
		btn_timer = millis();
		return true;
	}
	else return false;
}
boolean GButton::isHold() {
	if (step_flag) return true;
	else return false;
}

void GButton::tick() {	
	btn_state = !digitalRead(_BUTT) ^ _inv_state;
	if (btn_state) _state = true;
	else _state = false;
	
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