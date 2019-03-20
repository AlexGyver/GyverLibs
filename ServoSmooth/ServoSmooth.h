#ifndef ServoSmooth_h
#define ServoSmooth_h
#include <Arduino.h>
#include <Servo.h>

/*	
	ServoSmooth (by AlexGyver) - библиотека для плавного управления сервоприводами
	- Является дополнением к стандартной библиотеке Servo
	- Настройка максимальной скорости сервопривода
	- Настройка ускорения (разгон и торможение) сервопривода
	- Устанвока целевой позиции серво по углу (0-180) и длине импульса (500-2400)
	Текущая версия: 1.1 от 20.03.2019: автоматическое отключение (detach) при достижении цели
*/

#define SERVO_PERIOD 20		// период работы tick(), мс
#define DEADZONE 20			// мёртвая зона
#define TIMEOUT 100			// таймаут мёртвой зоны (в количестве периодов!!!)

class ServoSmooth {
	public:
		void write(uint16_t angle);					// аналог метода из библиотеки Servo
		void writeMicroseconds(uint16_t angle);		// аналог метода из библиотеки Servo
		void attach(uint8_t pin);					// аналог метода из библиотеки Servo
		void attach(uint8_t pin, int min, int max);	// аналог метода из библиотеки Servo. min по умолч. 500, max 2400
		void start();								// attach + разрешает работу tick
		void stop();								// detach + запрещает работу tick
		
		boolean tick();								// метод, управляющий сервой, должен опрашиваться как можно чаще.
													// Возвращает true, когда целевая позиция достигнута.
													// Имеет встроенный таймер с периодом SERVO_PERIOD
													
		boolean tickManual();						// метод, управляющий сервой, без встроенного таймера.
													// Возвращает true, когда целевая позиция достигнута
		
		void setSpeed(int speed);					// установка максимальной скорости (условные единицы, 0 - 200)
		void setAccel(float accel);					// установка ускорения (0.05 - 1). При значении 1 ускорение максимальное
		void setTarget(int target);					// установка целевой позиции в мкс (500 - 2400)
		void setTargetDeg(int target);				// установка целевой позиции в градусах (0-180). Зависит от min и max
	
		Servo _servo;
		int _servoCurrentPos = 0;
		int _servoTargetPos = 0;
		
	private:		
		int _min = 500;
		int _max = 2400;
		uint32_t _prevServoTime = 0;		
		int8_t _pin;
		int _servoMaxSpeed = 50;		
		int _newSpeed = 0;
		int _newPos = 0;
		float _k = 0.1;
		boolean _tickFlag = true;
		byte _timeoutCounter = 0;
		boolean _servoState = true;
};


#endif