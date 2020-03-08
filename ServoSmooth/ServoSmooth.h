#pragma once
#include <Arduino.h>
#include <Servo.h>

/*	
	ServoSmooth - библиотека для плавного управления сервоприводами
	- Является дополнением к стандартной библиотеке Servo
	- Настройка максимальной скорости сервопривода
	- Настройка ускорения (разгон и торможение) сервопривода
	- При использовании ESC и БК мотора получаем "плавный пуск" мотора
	- Устанвока целевой позиции серво по углу (0-180) и длине импульса (500-2400)
	- Автоматическое отключение (detach) при достижении цели
	Читай документацию здесь: https://alexgyver.ru/servosmooth/	
	
	v1.1 - автоматическое отключение (detach) при достижении цели
	v1.2 - вкл/выкл автоотключения серво
	v1.3 - отдельный метод для установки и чтения текущего положения. Добавлен вариант метода attach
	v1.4 - улучшена совместимость
	v1.5 - исправлены getCurrent и getCurrentDeg
	v1.6 - чуть оптимизирована инициализация
	v1.7 - исправлен баг с низкой скоростью/ускорением, код оптимизирован
	v1.8 - улучшена стабильность
	v1.9 - добавлена настройка макс. угла серво
	v1.10 - исправлен баг когда текущая позиция совпадает с позицией таргета
	v2.0 - упрощён алгоритм
	v2.1 - добавлена смена направления
	
	2019 by AlexGyver
*/

#define SS_SERVO_PERIOD 20		// период работы tick(), мс
#define SS_DEADZONE 10			// мёртвая зона
#define NORMAL 0
#define REVERSE 1

class ServoSmooth {
public:
	ServoSmooth(int maxAngle = 180);			// конструктор с передачей макс угла
	void write(uint16_t angle);					// аналог метода из библиотеки Servo
	void writeMicroseconds(uint16_t angle);		// аналог метода из библиотеки Servo
	void attach();								// подключает к выбранному в attach(pin) пину
	void attach(uint8_t pin, int target = 0);	// аналог метода из библиотеки Servo
	void attach(uint8_t pin, int min, int max, int target = 0);	// аналог метода из библиотеки Servo. min по умолч. 500, max 2400. target - положение (в углах или мкс, на которые серво повернётся при подключении)
	void detach();								// аналог метода из библиотеки Servo
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
	
	Servo _servo;		
	
private:
	void writeUs(int val);
	int _maxAngle = 180;
	int _servoCurrentPos = 0;
	int _servoTargetPos = 0;
	float _newPos = 0;
	int _min = 500;
	int _max = 2400;
	uint32_t _prevServoTime = 0;		
	int8_t _pin;
	int _servoMaxSpeed = 50;		
	int _newSpeed = 0;		
	float _k = 0.1;
	boolean _tickFlag = true;
	boolean _servoState = true;
	boolean _autoDetach = true;
	byte _timeoutCounter = 0;	
	bool _dir = 0;	
};