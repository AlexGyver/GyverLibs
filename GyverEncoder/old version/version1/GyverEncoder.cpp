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

void Encoder::invert() {
	uint8_t lol = _CLK;
	_CLK = _DT;
	_DT = lol;
}
void Encoder::setType(boolean type) {
	_type = type;
}
void Encoder::setCounters(int norm, int hold) {
	normCount = norm;
	holdCount = hold;
}
void Encoder::setCounterNorm(int norm) {
	normCount = norm;
}
void Encoder::setCounterHold(int hold) {
	holdCount = hold;
}

void Encoder::setSteps(int norm_step, int hold_step) {
	normCount_step = norm_step;
	holdCount_step = hold_step;
}
void Encoder::setStepNorm(int norm_step) {
	normCount_step = norm_step;
}
void Encoder::setStepHold(int hold_step) {
	holdCount_step = hold_step;
}

void Encoder::setLimitsNorm(int normMin, int normMax) {
	normCountMin = normMin;
	normCountMax = normMax;
}
void Encoder::setLimitsHold(int holdMin, int holdMax) {
	holdCountMin = holdMin;
	holdCountMax = holdMax;
}

boolean Encoder::isTurn() {
	if (isTurn_f) {
		isTurn_f = false;
		return true;
	} else return false;
}
boolean Encoder::isRight() {
	if (isRight_f) {
		isRight_f = false;
		return true;
	} else return false;
}
boolean Encoder::isLeft() {
	if (isLeft_f) {
		isLeft_f = false;
		return true;
	} else return false;
}
boolean Encoder::isRightH() {
	if (isRightH_f) {
		isRightH_f = false;
		return true;
	} else return false;
}
boolean Encoder::isLeftH() {
	if (isLeftH_f) {
		isLeftH_f = false;
		return true;
	} else return false;
}
boolean Encoder::isPress() {
	if (isPress_f) {
		isPress_f = false;
		return true;
	} else return false;
}
boolean Encoder::isRelease() {
	if (isRelease_f) {
		isRelease_f = false;
		return true;
	} else return false;
}
boolean Encoder::isHolded() {
	if (isHolded_f) {
		isHolded_f = false;
		return true;
	} else return false;
}
boolean Encoder::isHold() {
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
  if (SW_state && !butt_flag && millis() - debounce_timer > 200) {
    hold_flag = false;
    butt_flag = true;
    turn_flag = false;
    debounce_timer = millis();
    isPress_f = true;
  }
  if (!SW_state && butt_flag && millis() - debounce_timer > 200 && millis() - debounce_timer < 500) {
    butt_flag = false;
    if (!turn_flag && !hold_flag) {  // если кнопка отпущена и ручка не поворачивалась
      turn_flag = false;
      isRelease_f = true;
    }
    debounce_timer = millis();
  }

  if (SW_state && butt_flag && millis() - debounce_timer > 800 && !hold_flag) {
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

  if (DT_now != DT_last) {            // если предыдущее и текущее положение CLK разные, значит был поворот
	if (_type) _new_step = !_new_step;
	if (_new_step) {
    if (digitalRead(_DT) != DT_now) {  // если состояние DT отличается от CLK, значит крутим по часовой стрелке
      if (SW_state) {           // если кнопка энкодера нажата
        holdCount += holdCount_step;
		isRightH_f = true;
		isLeftH_f = false;
      } else {                  // если кнопка энкодера не нажата
        normCount += normCount_step;
        isRight_f = true;
		isLeft_f = false;
      }
    } else {                          // если совпадают, значит против часовой
      if (SW_state) {           // если кнопка энкодера нажата
        holdCount -= holdCount_step;
		isLeftH_f = true;
		isRightH_f = false;
      } else {                  // если кнопка энкодера не нажата
        normCount -= normCount_step;
        isLeft_f = true;
		isRight_f = false;
      }
    }
	}
	normCount = constrain(normCount, normCountMin, normCountMax);
	holdCount = constrain(holdCount, holdCountMin, holdCountMax);
    turn_flag = true;                    // флаг что был поворот ручки энкодера
	isTurn_f = true;
  }
  DT_last = DT_now;                   // обновить значение для энкодера
}