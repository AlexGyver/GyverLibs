#include "GyverButton.h"
#include <Arduino.h>

GButton::GButton(uint8_t BUTT) {
  _BUTT = BUTT;  
  _debounce = 80;
  _timeout = 500;
  _incr_timeout = 800;
  step = 1;
  pinMode (_BUTT, INPUT_PULLUP);
}

void GButton::setDebounce(uint8_t debounce) {
	_debounce = debounce;
}
void GButton::setTimeout(uint16_t timeout) {
	_timeout = timeout;
}
void GButton::setIncrStep(int16_t incr_step) {
	step = incr_step;
}
void GButton::setIncrTimeout(uint16_t incr_timeout) {
	_incr_timeout = incr_timeout;
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
boolean GButton::isHolded() {
	if (isHolded_f) {
		isHolded_f = false;
		return true;
	} else return false;
}
boolean GButton::isHold() {
	if (isHold_f) return true;
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

boolean GButton::isIncr() {
	if (incr_flag) return true;
	else return false;
}
int16_t GButton::getIncr(int16_t incr_value) {
	if (isHold_f && (millis() - incr_timer > _incr_timeout)) {
		incr_timer = millis();
		incr_flag = true;
		return (incr_value + step);	
	} else return incr_value;
}

void GButton::tick() {	
	btn_state = !digitalRead(_BUTT);
	if (btn_state) isHold_f = true;
	else isHold_f = false;
	
  if (btn_state && !btn_flag && (millis() - btn_timer > _debounce)) {
    btn_flag = true;
    btn_counter++;
    btn_timer = millis();
    isPress_f = true;
  }
  if (!btn_state && btn_flag) {
    btn_flag = false;
    hold_flag = false;
    isRelease_f = true;
	btn_timer = millis();
	incr_flag = false;
  }
  if (btn_flag && btn_state && (millis() - btn_timer > _timeout) && !hold_flag) {
    hold_flag = true;
    btn_counter = 0;
    isHolded_f = true;
	incr_flag = true;
	incr_timer = millis();
  }
  
  if ((millis() - btn_timer > _timeout) && (btn_counter != 0)) {    
    last_counter = btn_counter;
    btn_counter = 0;
	counter_flag = true;
  }
}