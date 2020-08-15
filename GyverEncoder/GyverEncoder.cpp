#include "GyverEncoder.h"
#include <Arduino.h>

#if defined(PRECISE_ALGORITHM)
const int8_t KNOBDIR[] = {
	0, -1,  1,  0,
	1,  0,  0, -1,
	-1,  0,  0,  1,
	0,  1, -1,  0,
};
int8_t encPos = 0;
#endif

// ================= CONSTRUCTOR =================
Encoder::Encoder() {
	flags.use_button = true;	
}

Encoder::Encoder(uint8_t clk, uint8_t dt, int8_t sw, bool type) {
	_CLK = clk;
	_DT = dt;
	if (sw != -1) {
		_SW = sw;
		flags.use_button = true;
	} else {
		flags.use_button = false;
	}	
	flags.enc_type = type;
	
#if defined(__AVR__)
	_pin_reg_CLK = portInputRegister(digitalPinToPort(_CLK));
	_bit_mask_CLK = digitalPinToBitMask(_CLK);
	_pin_reg_DT = portInputRegister(digitalPinToPort(_DT));
	_bit_mask_DT = digitalPinToBitMask(_DT);
	if (sw != -1) {
		_pin_reg_SW = portInputRegister(digitalPinToPort(_SW));
		_bit_mask_SW = digitalPinToBitMask(_SW);
	}
#endif
	
	pinMode(_CLK, (DEFAULT_ENC_PULL ? INPUT : INPUT_PULLUP));
	pinMode(_DT, (DEFAULT_ENC_PULL ? INPUT : INPUT_PULLUP));	
	if (flags.use_button) pinMode(_SW, (DEFAULT_BTN_PULL ? INPUT : INPUT_PULLUP));	
	flags.invBtn = (DEFAULT_BTN_PULL == HIGH_PULL) ? true : false;

#if defined(FAST_ALGORITHM)
	prevState = _readCLK();
#else
	prevState = _readCLK() | (_readDT() << 1);
#endif
}

// ================= SET =================
void Encoder::setDirection(bool direction) {
	if (direction) {
		uint8_t buf = _CLK;
		_CLK = _DT;
		_DT = buf;
	}
}
void Encoder::setPinMode(bool mode) {
	pinMode(_CLK, (mode) ? INPUT : INPUT_PULLUP);
	pinMode(_DT, (mode) ? INPUT : INPUT_PULLUP);
}
void Encoder::setBtnPinMode(bool mode) {
	pinMode(_SW, (mode) ? INPUT : INPUT_PULLUP);
	flags.invBtn = (mode) ? 0 : 1;
}
void Encoder::setType(bool type) {
	flags.enc_type = type;
}
void Encoder::setTickMode(bool tickMode) {
	flags.enc_tick_mode = tickMode;
}
void Encoder::setFastTimeout(uint16_t timeout) {
	_fast_timeout = timeout;
}

// ================= IS =================
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
boolean Encoder::isReleaseHold() {
	if (flags.enc_tick_mode) Encoder::tick();
	if (flags.isReleaseHold_f) {
		flags.isReleaseHold_f = false;
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
boolean Encoder::isSingle() {
	if (flags.enc_tick_mode) Encoder::tick();
	if (flags.isSingle_f) {
		flags.isSingle_f = false;
		flags.isDouble_f = false;
		return true;
	} else return false;
}
boolean Encoder::isDouble() {
	if (flags.enc_tick_mode) Encoder::tick();
	if (flags.isDouble_f) {
		flags.isDouble_f = false;
		flags.isSingle_f = false;
		return true;
	} else return false;
}
boolean Encoder::isHold() {
	if (flags.enc_tick_mode) Encoder::tick();
	return (SW_state);
}

void Encoder::resetStates() {
	encState = 0;
	flags.isTurn_f = false;
	flags.isFastR_f = false;
	flags.isFastL_f = false;
	flags.isPress_f = false;
	flags.isRelease_f = false;
	flags.isReleaseHold_f = false;
	flags.isHolded_f = false;
	flags.isSingle_f = false;
	flags.isDouble_f = false;
}

// ================= TICK =================
void Encoder::tick(bool clk, bool dt, bool sw) {
	extTick = true;
	flags.extCLK = clk;
	flags.extDT = dt;
	flags.extSW = sw;
	Encoder::tick();
	extTick = false;
}

void Encoder::tick() {
	uint32_t thisMls = millis();
	uint32_t debounceDelta = thisMls - debounce_timer;	

#ifdef ENC_WITH_BUTTON
	if (flags.use_button) {
		if (!extTick) SW_state = _readSW() ^ flags.invBtn;	// читаем состояние кнопки SW
		else SW_state = flags.extSW;
		
		if (SW_state && !flags.butt_flag && (debounceDelta > ENC_DEBOUNCE_BUTTON)) {
			flags.butt_flag = true;
			flags.turn_flag = false;
			debounce_timer = thisMls;
			debounceDelta = 0;
			flags.isPress_f = true;
			flags.isHolded_f = true;
			flags.doubleAllow = true;			
		}
		if (!SW_state && flags.butt_flag && (debounceDelta > ENC_DEBOUNCE_BUTTON)) {
			if (!flags.turn_flag && !flags.hold_flag) {  // если кнопка отпущена и ручка не поворачивалась
				flags.turn_flag = false;
				flags.isRelease_f = true;				
			}
			if (debounceDelta > ENC_HOLD_TIMEOUT) flags.isReleaseHold_f = true;
			flags.butt_flag = false;
			debounce_timer = thisMls;
			debounceDelta = 0;
			flags.hold_flag = false;
			
			if (flags.doubleAllow && !flags.doubleFlag) {
				flags.doubleFlag = true;
				flags.countFlag = false;
			} else {
				flags.countFlag = true;
			}
		}
		if (flags.doubleFlag && debounceDelta > ENC_DOUBLE_TIMEOUT) {	
			if (!flags.turn_flag) {
				if (!flags.countFlag) flags.isSingle_f = true;
				else flags.isDouble_f = true;
			}			
			flags.doubleFlag = false;
		}
		if (flags.butt_flag && debounceDelta > ENC_HOLD_TIMEOUT && !flags.turn_flag) {
			if (SW_state) {
				flags.hold_flag = true;				
				flags.doubleAllow = false;
			} else {
				flags.butt_flag = false;
				flags.hold_flag = false;				
				debounce_timer = thisMls;
				debounceDelta = 0;
			}	
		}
	}
#endif

#if defined(FAST_ALGORITHM)
	uint8_t curState = (extTick) ? (flags.extCLK) : (_readCLK());
	
	if (curState != prevState
#if (ENC_DEBOUNCE_TURN > 0)
	&& (debounceDelta > ENC_DEBOUNCE_TURN)
#endif
	) {
		encState = 0;
		turnFlag = !turnFlag;
		if (turnFlag || !flags.enc_type) {
			if (( (extTick) ? (flags.extDT) : _readDT() ) != prevState) {
				encState = 1;
			} else {
				encState = 2;
			}
		}

#elif defined(BINARY_ALGORITHM)		
		uint8_t curState = (extTick) ? (flags.extCLK | (flags.extDT << 1)) : (_readCLK() | (_readDT() << 1));
		
		if (curState != prevState
#if (ENC_DEBOUNCE_TURN > 0)
		&& (debounceDelta > ENC_DEBOUNCE_TURN)
#endif
		) {			
			encState = 0;
			if (flags.rst_flag) {
				if (curState == 0b11) {
					flags.rst_flag = 0;
					//encState = 3-prevState;
					switch (prevState) {
					case 0b10: encState = 1; break;		// 2 - 1
					case 0b01: encState = 2; break;		// 1 - 2
					}
				} else if (!flags.enc_type && (curState == 0b00)) {
					flags.rst_flag = 0;
					//encState = prevState;
					switch (prevState) {
					case 0b01: encState = 1; break;
					case 0b10: encState = 2; break;
					}
				}
			}
			if (curState == 0b00 || (!flags.enc_type && curState == 0b11)) flags.rst_flag = 1;

#elif defined(PRECISE_ALGORITHM)			
			uint8_t curState = (extTick) ? (flags.extCLK | (flags.extDT << 1)) : (_readCLK() | (_readDT() << 1));

			if (curState != prevState
#if (ENC_DEBOUNCE_TURN > 0)
			&& (debounceDelta > ENC_DEBOUNCE_TURN)
#endif
			) {
				encState = 0;
				encPos += KNOBDIR[curState | (prevState << 2)];
				if (flags.enc_type) {
					if (curState == 0x3 && encPos != 0) {
						encState = (encPos == 4) ? 1 : 2;
						encPos = 0;
					}
				} else {
					if ((curState == 0x3 || !curState) && encPos != 0) {
						encState = (encPos == 2) ? 1 : 2;
						encPos = 0;
					}
				}				
#endif
				
				if (encState != 0) {
					flags.isTurn_f = true;
					if (thisMls - fast_timer < _fast_timeout) {
						if (encState == 1) flags.isFastL_f = true;
						else if (encState == 2) flags.isFastR_f = true;
						fast_timer = thisMls;
					} else fast_timer = thisMls;
#ifdef ENC_WITH_BUTTON
					if (flags.use_button) if (SW_state) encState += 2;
#endif
				}		
				prevState = curState;
				flags.turn_flag = true;
				debounce_timer = thisMls;
				debounceDelta = 0;
			}
		}