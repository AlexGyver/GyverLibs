#ifndef smoothUtil_h
#define smoothUtil_h
// базовый класс для плавного управления серво
// в дочернем нужно объявить sendToDriver(int), attach(int) и detach()

#include <Arduino.h>

#define SS_SERVO_PERIOD 20		// период работы tick(), мс
#define SS_DEADTIME 10			// количество тиков до detach
#define NORMAL 0
#define REVERSE 1

class Smooth {
public:
	void write(uint16_t angle);					// повернуть на угол. Аналог метода из библиотеки Servo
	virtual void sendToDriver(uint16_t val);	// отправить на серво
	void writeMicroseconds(uint16_t angle);		// повернуть на импульс. Аналог метода из библиотеки Servo
	void attach();
	virtual void attach(int pin);				// аналог метода из библиотеки Servo	
	void attach(int pin, int target);			// аттач + установка позиции (в градусах ИЛИ в микросекундах, программа сама поймёт)
	void attach(int pin, int min, int max, int target = 0);	// аналог метода из библиотеки Servo. min по умолч. 500, max 2400. target - положение (в углах или мкс, на которые серво повернётся при подключении)
	virtual void detach();						// аналог метода из библиотеки Servo
	void start();								// attach + разрешает работу tick
	void stop();								// detach + запрещает работу tick
	
	boolean tick();								// метод, управляющий сервой, должен опрашиваться как можно чаще.
	// Возвращает true, когда целевая позиция достигнута.
	// Имеет встроенный таймер с периодом SS_SERVO_PERIOD
	
	boolean tickManual();						// метод, управляющий сервой, без встроенного таймера.
	// Возвращает true, когда целевая позиция достигнута
	
	void setSpeed(int speed);					// установка максимальной скорости (больше 0), градусов / с
	void setAccel(double accel);				// установка ускорения (0.1-1), условные величины
	void setAccel(int accel);					// установка ускорения в градусах/сек/сек (рабочее от 0 до ~1500)
	void setTarget(int target);					// установка целевой позиции в мкс (~500 - 2400 серво, ~150-600 драйвер PCA9685)
	void setTargetDeg(int target);				// установка целевой позиции в градусах (0-макс. угол). Зависит от min и max
	void setAutoDetach(boolean set);			// вкл/выкл автоматического отключения (detach) при достижении угла. По умолч. вкл
	void setCurrent(int target);				// установка текущей позиции в мкс (~500 - 2400 серво, ~150-600 драйвер PCA9685)
	void setCurrentDeg(int target);				// установка текущей позиции в градусах (0-макс. угол). Зависит от min и max
	void setMaxAngle(int maxAngle);				// установка макс. угла привода
	int getCurrent();							// получение текущей позиции в мкс (~500 - 2400 серво, ~150-600 драйвер PCA9685)
	int getCurrentDeg();						// получение текущей позиции в градусах (0-макс. угол). Зависит от min и max
	int getTarget();							// получение целевой позиции в мкс (~500 - 2400 серво, ~150-600 драйвер PCA9685)
	int getTargetDeg();							// получение целевой позиции в градусах (0-макс. угол). Зависит от min и max
	
	void setDirection(bool _dir);				// смена направления поворота
	void smoothStart();							// вызывать сразу после attach(пин, таргет). Смягчает движение серво из неизвестной позиции к стартовой. БЛОКИРУЮЩАЯ НА 1С!
	
protected:
	void writeUs(int val);
	float _speed = 0, _lastSpeed = 0;
	byte timeoutCounter = 0;
	int _maxAngle = 180;
	int _servoCurrentPos = 600;	// если не вызван аттач(пин, таргет)
	int _servoTargetPos = 600;
	int _min = 500;
	int _max = 2400;
	float _delta = SS_SERVO_PERIOD / 1000.0;
	uint32_t _prevServoTime = 0;		
	int8_t _pin;
	int16_t _servoMaxSpeed = 1400;
	uint16_t _acceleration = 1000;
	bool _tickFlag = true;
	bool _servoState = true;
	bool _autoDetach = true;
	bool _dir = 0;
	byte SS_DEADZONE = 10;
	byte SS_DEADZONE_SP = 3;
};
#endif