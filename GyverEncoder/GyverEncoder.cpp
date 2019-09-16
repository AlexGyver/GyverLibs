#include "GyverEncoder.h"
#include <Arduino.h>

Encoder::Encoder(uint8_t clk, uint8_t dt) {
	_CLK = clk;
	_DT = dt;
	flags.use_button = false;
	Encoder::init();
}

Encoder::Encoder(uint8_t clk, uint8_t dt, uint8_t sw) {
	_CLK = clk;
	_DT = dt;
	_SW = sw;
	flags.use_button = true;
	Encoder::init();
}
Encoder::Encoder(uint8_t clk, uint8_t dt, uint8_t sw, uint8_t type) {
	_CLK = clk;
	_DT = dt;
	_SW = sw;
	flags.use_button = true;
	flags.enc_type = type;
	Encoder::init();
}

void Encoder::init() {
	pinMode (_CLK, INPUT);
	pinMode (_DT, INPUT);
	if (flags.use_button) pinMode(_SW, INPUT_PULLUP);
	
	curState = digitalRead(_CLK);
	curState += digitalRead(_DT) << 1;
	prevState = curState;
}

void Encoder::setDirection(uint8_t direction) {
	if (direction) {
		uint8_t buf = _CLK;
		_CLK = _DT;
		_DT = buf;
	}
}
void Encoder::setType(uint8_t type) {
	flags.enc_type = type;
}
void Encoder::setTickMode(uint8_t tickMode) {
	flags.enc_tick_mode = tickMode;
}
void Encoder::setFastTimeout(int timeout) {
	fast_timeout = timeout;
}

// повороты
boolean Encoder::isTurn() {
	if (flags.enc_tick_mode) Encoder::tick();
	if (flags.isTurn_f) {
		flags.isTurn_f = false;
		return true;
	} else return false;
}
boolean Encoder::isRight() {
	if (flags.enc_tick_mode) Encoder::tick();
	if (encState == 2) {
		encState = 0;
		return true;
	} else return false;
}
boolean Encoder::isLeft() {
	if (flags.enc_tick_mode) Encoder::tick();
	if (encState == 1) {
		encState = 0;
		return true;
	} else return false;
}
boolean Encoder::isRightH() {
	if (flags.enc_tick_mode) Encoder::tick();
	if (encState == 4) {
		encState = 0;
		return true;
	} else return false;
}
boolean Encoder::isLeftH() {
	if (flags.enc_tick_mode) Encoder::tick();
	if (encState == 3) {
		encState = 0;
		return true;
	} else return false;
}
boolean Encoder::isFastR() {
	if (flags.enc_tick_mode) Encoder::tick();
	if (flags.isFastR_f) {
		flags.isFastR_f = false;
		return true;
	} else return false;
}
boolean Encoder::isFastL() {
	if (flags.enc_tick_mode) Encoder::tick();
	if (flags.isFastL_f) {
		flags.isFastL_f = false;
		return true;
	} else return false;
}

// кнопка
boolean Encoder::isPress() {
	if (flags.enc_tick_mode) Encoder::tick();
	if (flags.isPress_f) {
		flags.isPress_f = false;
		return true;
	} else return false;
}
boolean Encoder::isRelease() {
	if (flags.enc_tick_mode) Encoder::tick();
	if (flags.isRelease_f) {
		flags.isRelease_f = false;
		return true;
	} else return false;
}
boolean Encoder::isClick() {
	if (flags.enc_tick_mode) Encoder::tick();
	if (flags.isRelease_f) {
		flags.isRelease_f = false;
		return true;
	} else return false;
}
boolean Encoder::isHolded() {
	if (flags.enc_tick_mode) Encoder::tick();
	if (flags.hold_flag && flags.isHolded_f) {
		flags.isHolded_f = false;
		return true;
	} else return false;
}
boolean Encoder::isHold() {
	if (flags.enc_tick_mode) Encoder::tick();
	return (flags.SW_state);
}

void Encoder::tick() {
	uint32_t debounceDelta = millis() - debounce_timer;
	
	if (flags.use_button) {
		flags.SW_state = !digitalRead(_SW);        // читаем положение кнопки SW
		if (flags.SW_state && !flags.butt_flag && (debounceDelta > DEBOUNCE_BUTTON)) {
			flags.butt_flag = true;
			flags.turn_flag = false;
			debounce_timer = millis();
			debounceDelta = 0;
			flags.isPress_f = true;
			flags.isHolded_f = true;
		}
		if (!flags.SW_state && flags.butt_flag && (debounceDelta > DEBOUNCE_BUTTON)) {
			if (!flags.turn_flag && !flags.hold_flag) {  // если кнопка отпущена и ручка не поворачивалась
				flags.turn_flag = false;
				flags.isRelease_f = true;
			}
			flags.butt_flag = false;
			debounce_timer = millis();
			debounceDelta = 0;
			flags.hold_flag = false;
		}
		if (flags.butt_flag && debounceDelta > HOLD_TIMEOUT && !flags.turn_flag) {
			if (flags.SW_state) {
				flags.hold_flag = true;
				flags.isRelease_f = false;
			} else {
				flags.butt_flag = false;
				flags.hold_flag = false;
				debounce_timer = millis();
				debounceDelta = 0;
			}
		}
	}

	// читаем состояние энкодера
	curState = digitalRead(_CLK);
	curState += digitalRead(_DT) << 1;
	
	if (curState != prevState && (debounceDelta > DEBOUNCE_TURN)) {		
		encState = 0;
		if (curState == 0b11) {
			if (prevState == 0b10) encState = 1;
			else if (prevState == 0b01) encState = 2;
		} else if (curState == 0b00 && !flags.enc_type) {
			if (prevState == 0b01) encState = 1;
			else if (prevState == 0b10) encState = 2;
		}
		if (encState != 0) {
			flags.isTurn_f = true;
			if (millis() - fast_timer < fast_timeout) {
				if (encState == 1) flags.isFastL_f = true;
				else if (encState == 2) flags.isFastR_f = true;
				fast_timer = millis();
			} else fast_timer = millis();
			if (flags.SW_state) encState += 2;
		}		
		prevState = curState;
		flags.turn_flag = true;
		debounce_timer = millis();
		debounceDelta = 0;
	}
}