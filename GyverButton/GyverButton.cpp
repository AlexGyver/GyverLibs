#include "GyverButton.h"
#include <Arduino.h>

GButton::GButton(uint8_t pin) {
	_PIN = pin;
	GButton::init();
}
GButton::GButton(uint8_t pin, uint8_t type, uint8_t dir) {
	_PIN = pin;
	GButton::init();
	GButton::setType(type);
	flags.inv_state = dir;
}

void GButton::init() {
	_debounce = 60;
	_timeout = 500;
	_step_timeout = 400;
	_click_timeout = 300;
	flags.inv_state = NORM_OPEN;
	flags.mode = false;
	flags.type = false;
	flags.tickMode = false;
	GButton::setType(HIGH_PULL);
}

void GButton::setDebounce(uint16_t debounce) {
	_debounce = debounce;
}
void GButton::setTimeout(uint16_t timeout) {
	_timeout = timeout;
}
void GButton::setClickTimeout(uint16_t timeout) {
	_click_timeout = timeout;
}
void GButton::setStepTimeout(uint16_t step_timeout) {
	_step_timeout = step_timeout;
}
void GButton::setType(uint8_t type) {
	flags.type = type;
	if (type) pinMode(_PIN, INPUT);
	else pinMode(_PIN, INPUT_PULLUP);
}
void GButton::setDirection(uint8_t dir) {
	flags.inv_state = dir;
}
void GButton::setTickMode(uint8_t tickMode) {
	flags.tickMode = tickMode;
}

boolean GButton::isPress() {
	if (flags.tickMode) GButton::tick();
	if (flags.isPress_f) {
		flags.isPress_f = false;
		return true;
	} else return false;
}
boolean GButton::isRelease() {
	if (flags.tickMode) GButton::tick();
	if (flags.isRelease_f) {
		flags.isRelease_f = false;
		return true;
	} else return false;
}
boolean GButton::isClick() {	
	if (flags.tickMode) GButton::tick();
	if (flags.isOne_f) {
		flags.isOne_f = false;
		return true;
	} else return false;
}
boolean GButton::isHolded() {
	if (flags.tickMode) GButton::tick();
	if (flags.isHolded_f) {
		flags.isHolded_f = false;
		return true;
	} else return false;
}
boolean GButton::isHold() {
	if (flags.tickMode) GButton::tick();
	if (flags.step_flag) return true;
	else return false;
}
boolean GButton::state() {
	if (flags.tickMode) GButton::tick();
	return flags.btn_state;
}
boolean GButton::isSingle() {
	if (flags.tickMode) GButton::tick();
	if (flags.counter_flag && last_counter == 1) {
		flags.counter_flag = false;
		return true;
	} else return false;
}
boolean GButton::isDouble() {
	if (flags.tickMode) GButton::tick();
	if (flags.counter_flag && last_counter == 2) {
		flags.counter_flag = false;
		return true;
	} else return false;
}
boolean GButton::isTriple() {
	if (flags.tickMode) GButton::tick();
	if (flags.counter_flag && last_counter == 3) {
		flags.counter_flag = false;
		return true;
	} else return false;
}
boolean GButton::hasClicks() {
	if (flags.tickMode) GButton::tick();
	if (flags.counter_flag) {
		flags.counter_flag = false;
		return true;
	} else return false;
}
uint8_t GButton::getClicks() {
	return last_counter;	
}
boolean GButton::isStep() {
	if (flags.tickMode) GButton::tick();
	if (flags.step_flag && (millis() - btn_timer >= _step_timeout)) {
		btn_timer = millis();
		return true;
	}
	else return false;
}
void GButton::tick(boolean state) {
	flags.mode = true;
	flags.btn_state = state ^ flags.inv_state;
	GButton::tick();
	flags.mode = false;
}
void GButton::tick() {	
  // читаем пин
  if (!flags.mode) flags.btn_state = !digitalRead(_PIN) ^ (flags.inv_state ^ flags.type);
	
  // нажатие
  if (flags.btn_state && !flags.btn_flag) {
	if (!flags.btn_deb) {
		flags.btn_deb = true;
		btn_timer = millis();
	} else {
		if (millis() - btn_timer >= _debounce) {
			flags.btn_flag = true;			
			flags.isPress_f = true;
			flags.oneClick_f = true;
		}
	}    
  } else {
	  flags.btn_deb = false;
  }
  
  // отпускание
  if (!flags.btn_state && flags.btn_flag) {
    flags.btn_flag = false;
	if (!flags.hold_flag) btn_counter++;
    flags.hold_flag = false;
    flags.isRelease_f = true;
	btn_timer = millis();
	flags.step_flag = false;	
	if (flags.oneClick_f) {
		flags.oneClick_f = false;
		flags.isOne_f = true;
	}
  }
  
  // кнопка удерживается
  if (flags.btn_flag && flags.btn_state && (millis() - btn_timer >= _timeout) && !flags.hold_flag) {
    flags.hold_flag = true;
    btn_counter = 0;
	last_counter = 0;
    flags.isHolded_f = true;
	flags.step_flag = true;
	flags.oneClick_f = false;
	btn_timer = millis();
  }
  
  // обработка накликивания
  if ((millis() - btn_timer >= _click_timeout) && (btn_counter != 0)) {    
    last_counter = btn_counter;
    btn_counter = 0;
	flags.counter_flag = true;
  }
}