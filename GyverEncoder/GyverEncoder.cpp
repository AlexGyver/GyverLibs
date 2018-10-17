#include "GyverEncoder.h"
#include <Arduino.h>

Encoder::Encoder(uint8_t clk, uint8_t dt, uint8_t sw) {
	_CLK = clk;
	_DT = dt;
	_SW = sw;
	Encoder::init();
}
Encoder::Encoder(uint8_t clk, uint8_t dt, uint8_t sw, boolean type) {
	_CLK = clk;
	_DT = dt;
	_SW = sw;
	_type = type;
	Encoder::init();	
}

void Encoder::init() {
	pinMode (_CLK, INPUT);
	pinMode (_DT, INPUT);
	pinMode (_SW, INPUT_PULLUP);
	flags.DT_last = digitalRead(_CLK);         // читаем начальное положение CLK
}

void Encoder::setDirection(boolean direction) {
	if (_direction != direction) {
		_direction = direction;
		uint8_t lol = _CLK;
		_CLK = _DT;
		_DT = lol;
	}	
}
void Encoder::setType(boolean type) {
	_type = type;
}
void Encoder::setTickMode(boolean tickMode) {
	_tickMode = tickMode;
}
boolean Encoder::isTurn() {
	if (_tickMode) Encoder::tick();
	if (flags.isTurn_f) {
		flags.isTurn_f = false;
		return true;
	} else return false;
}
boolean Encoder::isRight() {
	if (_tickMode) Encoder::tick();
	if (flags.isRight_f) {
		flags.isRight_f = false;
		return true;
	} else return false;
}
boolean Encoder::isLeft() {
	if (_tickMode) Encoder::tick();
	if (flags.isLeft_f) {
		flags.isLeft_f = false;
		return true;
	} else return false;
}
boolean Encoder::isRightH() {
	if (_tickMode) Encoder::tick();
	if (flags.isRightH_f) {
		flags.isRightH_f = false;
		return true;
	} else return false;
}
boolean Encoder::isLeftH() {
	if (_tickMode) Encoder::tick();
	if (flags.isLeftH_f) {
		flags.isLeftH_f = false;
		return true;
	} else return false;
}
boolean Encoder::isPress() {
	if (_tickMode) Encoder::tick();
	if (flags.isPress_f) {
		flags.isPress_f = false;
		return true;
	} else return false;
}
boolean Encoder::isRelease() {
	if (_tickMode) Encoder::tick();
	if (flags.isRelease_f) {
		flags.isRelease_f = false;
		return true;
	} else return false;
}
boolean Encoder::isClick() {
	if (_tickMode) Encoder::tick();
	if (flags.isRelease_f) {
		flags.isRelease_f = false;
		return true;
	} else return false;
}
boolean Encoder::isHolded() {
	if (_tickMode) Encoder::tick();
	if (flags.isHolded_f) {
		flags.isHolded_f = false;
		return true;
	} else return false;
}
boolean Encoder::isHold() {
	if (_tickMode) Encoder::tick();
	if (flags.isHold_f) {
		flags.isHold_f = false;
		return true;
	} else return false;
}

void Encoder::tick() {	
  flags.DT_now = digitalRead(_CLK);          // читаем текущее положение CLK
  flags.SW_state = !digitalRead(_SW);        // читаем положение кнопки SW
  if (flags.SW_state) flags.isHold_f = true;
  else flags.isHold_f = false;

  // отработка нажатия кнопки энкодера
  if (flags.SW_state && !flags.butt_flag && millis() - debounce_timer > debounce_button) {
    flags.hold_flag = false;
    flags.butt_flag = true;
    flags.turn_flag = false;
    debounce_timer = millis();
    flags.isPress_f = true;
  }
  if (!flags.SW_state && flags.butt_flag && millis() - debounce_timer > debounce_button && millis() - debounce_timer < 500) {
    flags.butt_flag = false;
    if (!flags.turn_flag && !flags.hold_flag) {  // если кнопка отпущена и ручка не поворачивалась
      flags.turn_flag = false;
      flags.isRelease_f = true;
    }
    debounce_timer = millis();
  }

  if (flags.SW_state && flags.butt_flag && millis() - debounce_timer > hold_timer && !flags.hold_flag) {
    flags.hold_flag = true;
    if (!flags.turn_flag) {  // если кнопка отпущена и ручка не поворачивалась
      flags.turn_flag = false;
      flags.isHolded_f = true;
    }
  }
  if (!flags.SW_state && flags.butt_flag && flags.hold_flag) {
    flags.butt_flag = false;
    debounce_timer = millis();
  }
	
  // если предыдущее и текущее положение CLK разные, значит был поворот
  if (flags.DT_now != flags.DT_last && (millis() - debounce_timer > debounce_turn)) {
	debounce_timer = millis();
	if (_type) _new_step = !_new_step;
	if (_new_step) {
    if (digitalRead(_DT) != flags.DT_now) {  // если состояние DT отличается от CLK, значит крутим по часовой стрелке
      if (flags.SW_state) {           // если кнопка энкодера нажата
		flags.isRightH_f = true;
		flags.isLeftH_f = false;
      } else {                  // если кнопка энкодера не нажата
        flags.isRight_f = true;
		flags.isLeft_f = false;
      }
    } else {                          // если совпадают, значит против часовой
      if (flags.SW_state) {           // если кнопка энкодера нажата
		flags.isLeftH_f = true;
		flags.isRightH_f = false;
      } else {                  // если кнопка энкодера не нажата
        flags.isLeft_f = true;
		flags.isRight_f = false;
      }
    }
	flags.isTurn_f = true;
	}
    flags.turn_flag = true;                    // флаг что был поворот ручки энкодера	
  }
  flags.DT_last = flags.DT_now;                   // обновить значение для энкодера
}