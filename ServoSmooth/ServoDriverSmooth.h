#pragma once
#include <Arduino.h>
#include "Adafruit_PWMServoDriver.h"
#include "smoothUtil.h"
#include <Wire.h>

// полное описание библиотеки смотри в ServoSmooth.h

class ServoDriverSmooth : public Smooth {
public:
	ServoDriverSmooth(uint8_t addr = 0x40, int maxAngle = 180);
	using Smooth::attach;
	void attach(int pin);
	void detach();
	void sendToDriver(uint16_t val);
	Adafruit_PWMServoDriver _servo = Adafruit_PWMServoDriver();
private:
	bool _startFlag = false;
};