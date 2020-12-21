#ifndef ServoDriverSmooth_h
#define ServoDriverSmooth_h
#include <Arduino.h>
#include "Adafruit_PWMServoDriver.h"
#include "smoothUtil.h"
#include <Wire.h>

#define _DRIVER_DEADZONE 5			// мёртвая зона (по микросекундам)
#define _DRIVER_DEADZONE_SP 1		// мёртвая зона (по скорости)

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
	static bool _startFlag;		// общий флаг для всех членов
};
#endif