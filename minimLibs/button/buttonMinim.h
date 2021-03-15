#ifndef buttonMinim_h
#define buttonMinim_h

// мини-класс для работы с кнопкой, версия 1.0
typedef struct {
	bool holdedFlag: 1;
	bool btnFlag: 1;
	bool pressF: 1;
	bool clickF: 1;
	bool holdF: 1;
} buttonMinimFlags;

class buttonMinim {
public:
	buttonMinim(uint8_t pin) {
		pinMode(pin, INPUT_PULLUP);
		_pin = pin;
	}
	boolean pressed() {
		tick();
		if (flags.pressF) {
			flags.pressF = false;
			return true;
		}
		else return false;
	}
	boolean clicked() {
		tick();
		if (flags.clickF) {
			flags.clickF = false;
			return true;
		}
		else return false;
	}
	boolean holding() {
		tick();
		if (flags.holdF) {
			return true;
		}
		else return false;
	}
	boolean holded() {
		tick();
		if (flags.holdF && flags.holdedFlag) {
			flags.holdedFlag = false;
			return true;
		}
		else return false;
	}
private:
	buttonMinimFlags flags;
	void tick() {
		boolean btnState = digitalRead(_pin);
		if (!btnState && !flags.btnFlag && ((uint32_t)millis() - _btnTimer > 90)) {
			flags.btnFlag = true;
			_btnTimer = millis();
			flags.pressF = true;
			flags.holdedFlag = true;
		}
		if (btnState && flags.btnFlag && ((uint32_t)millis() - _btnTimer < 350)) {
			flags.btnFlag = false;
			_btnTimer = millis();
			flags.clickF = true;
			flags.holdF = false;
		}
		if (flags.btnFlag && ((uint32_t)millis() - _btnTimer > 600)) {
			if (!btnState) {
				flags.holdF = true;
			} else {
				flags.btnFlag = false;
				flags.holdF = false;
				_btnTimer = millis();
			}
		}
	}
	uint32_t _btnTimer;
	byte _pin;
};
#endif