#pragma once
// базовый класс для плавного управления серво
// в дочернем нужно объявить sendToDriver(int), attach(int) и detach()

#include <Arduino.h>

#define SS_SERVO_PERIOD 20		// период работы tick(), мс
#define SS_DEADZONE 10			// мёртвая зона (по микросекундам)
#define NORMAL 0
#define REVERSE 1

class Smooth {
public:
	void write(uint16_t angle);					// повернуть на угол. Аналог метода из библиотеки Servo
	virtual void sendToDriver(uint16_t val);	// отправить на серво
	void writeMicroseconds(uint16_t angle);		// повернуть на импульс. Аналог метода из библиотеки Servo
	void attach();
	virtual void attach(int pin);				// аналог метода из библиотеки Servo	
	void attach(int pin, int target);			// аттач+установка позиции
	void attach(int pin, int min, int max, int target = 0);	// аналог метода из библиотеки Servo. min по умолч. 500, max 2400. target - положение (в углах или мкс, на которые серво повернётся при подключении)
	virtual void detach();						// аналог метода из библиотеки Servo
	void start();								// attach + разрешает работу tick
	void stop();								// detach + запрещает работу tick
	
	boolean tick();								// метод, управляющий сервой, должен опрашиваться как можно чаще.
	// Возвращает true, когда целевая позиция достигнута.
	// Имеет встроенный таймер с периодом SS_SERVO_PERIOD
	
	boolean tickManual();						// метод, управляющий сервой, без встроенного таймера.
	// Возвращает true, когда целевая позиция достигнута
	
	void setSpeed(int speed);					// установка максимальной скорости (условные единицы, 0 - 200)
	void setAccel(float accel);					// установка ускорения (0.05 - 1). При значении 1 ускорение максимальное
	void setTarget(int target);					// установка целевой позиции в мкс (500 - 2400)
	void setTargetDeg(int target);				// установка целевой позиции в градусах (0-макс. угол). Зависит от min и max
	void setAutoDetach(boolean set);			// вкл/выкл автоматического отключения (detach) при достижении угла. По умолч. вкл
	void setCurrent(int target);				// установка текущей позиции в мкс (500 - 2400)
	void setCurrentDeg(int target);				// установка текущей позиции в градусах (0-макс. угол). Зависит от min и max
	void setMaxAngle(int maxAngle);				// установка макс. угла привода
	int getCurrent();							// получение текущей позиции в мкс (500 - 2400)
	int getCurrentDeg();						// получение текущей позиции в градусах (0-макс. угол). Зависит от min и max
	int getTarget();							// получение целевой позиции в мкс (500 - 2400)
	int getTargetDeg();							// получение целевой позиции в градусах (0-макс. угол). Зависит от min и max
	
	void setDirection(bool _dir);				// смена направления поворота
	
protected:
	void writeUs(int val);
	int _maxAngle = 180;
	int _servoCurrentPos = 0;
	int _servoTargetPos = 0;
	int _min = 500;
	int _max = 2400;
	float _speed = 0;
	uint32_t _prevServoTime = 0;		
	int8_t _pin;
	int _servoMaxSpeed = 100;			
	float _k = 0.5;
	boolean _tickFlag = true;
	boolean _servoState = true;
	boolean _autoDetach = true;
	bool _dir = 0;
};