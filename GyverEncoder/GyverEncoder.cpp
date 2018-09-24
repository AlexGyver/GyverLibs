#include "GyverEncoder.h"
#include <Arduino.h>

Encoder::Encoder(uint8_t CLK, uint8_t DT, uint8_t SW) {
  _CLK = CLK;
  _DT = DT;
  _SW = SW;
  pinMode (_CLK, INPUT);
  pinMode (_DT, INPUT);
  pinMode (_SW, INPUT_PULLUP);
  DT_last = digitalRead(_CLK);         // читаем начальное положение CLK
}
Encoder::Encoder(uint8_t CLK, uint8_t DT, uint8_t SW, boolean type) {
	Encoder::Encoder(CLK, DT, SW);
	_type = type;
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
	if (isTurn_f) {
		isTurn_f = false;
		return true;
	} else return false;
}
boolean Encoder::isRight() {
	if (_tickMode) Encoder::tick();
	if (isRight_f) {
		isRight_f = false;
		return true;
	} else return false;
}
boolean Encoder::isLeft() {
	if (_tickMode) Encoder::tick();
	if (isLeft_f) {
		isLeft_f = false;
		return true;
	} else return false;
}
boolean Encoder::isRightH() {
	if (_tickMode) Encoder::tick();
	if (isRightH_f) {
		isRightH_f = false;
		return true;
	} else return false;
}
boolean Encoder::isLeftH() {
	if (_tickMode) Encoder::tick();
	if (isLeftH_f) {
		isLeftH_f = false;
		return true;
	} else return false;
}
boolean Encoder::isPress() {
	if (_tickMode) Encoder::tick();
	if (isPress_f) {
		isPress_f = false;
		return true;
	} else return false;
}
boolean Encoder::isRelease() {
	if (_tickMode) Encoder::tick();
	if (isRelease_f) {
		isRelease_f = false;
		return true;
	} else return false;
}
boolean Encoder::isHolded() {
	if (_tickMode) Encoder::tick();
	if (isHolded_f) {
		isHolded_f = false;
		return true;
	} else return false;
}
boolean Encoder::isHold() {
	if (_tickMode) Encoder::tick();
	if (isHold_f) {
		isHold_f = false;
		return true;
	} else return false;
}

void Encoder::tick() {	
  DT_now = digitalRead(_CLK);          // читаем текущее положение CLK
  SW_state = !digitalRead(_SW);        // читаем положение кнопки SW
  if (SW_state) isHold_f = true;
  else isHold_f = false;

  // отработка нажатия кнопки энкодера
  if (SW_state && !butt_flag && millis() - debounce_timer > debounce_button) {
    hold_flag = false;
    butt_flag = true;
    turn_flag = false;
    debounce_timer = millis();
    isPress_f = true;
  }
  if (!SW_state && butt_flag && millis() - debounce_timer > debounce_button && millis() - debounce_timer < 500) {
    butt_flag = false;
    if (!turn_flag && !hold_flag) {  // если кнопка отпущена и ручка не поворачивалась
      turn_flag = false;
      isRelease_f = true;
    }
    debounce_timer = millis();
  }

  if (SW_state && butt_flag && millis() - debounce_timer > hold_timer && !hold_flag) {
    hold_flag = true;
    if (!turn_flag) {  // если кнопка отпущена и ручка не поворачивалась
      turn_flag = false;
      isHolded_f = true;
    }
  }
  if (!SW_state && butt_flag && hold_flag) {
    butt_flag = false;
    debounce_timer = millis();
  }
	
  // если предыдущее и текущее положение CLK разные, значит был поворот
  if (DT_now != DT_last && (millis() - debounce_timer > debounce_turn)) {
	debounce_timer = millis();
	if (_type) _new_step = !_new_step;
	if (_new_step) {
    if (digitalRead(_DT) != DT_now) {  // если состояние DT отличается от CLK, значит крутим по часовой стрелке
      if (SW_state) {           // если кнопка энкодера нажата
		isRightH_f = true;
		isLeftH_f = false;
      } else {                  // если кнопка энкодера не нажата
        isRight_f = true;
		isLeft_f = false;
      }
    } else {                          // если совпадают, значит против часовой
      if (SW_state) {           // если кнопка энкодера нажата
		isLeftH_f = true;
		isRightH_f = false;
      } else {                  // если кнопка энкодера не нажата
        isLeft_f = true;
		isRight_f = false;
      }
    }
	}
    turn_flag = true;                    // флаг что был поворот ручки энкодера
	isTurn_f = true;
  }
  DT_last = DT_now;                   // обновить значение для энкодера
}