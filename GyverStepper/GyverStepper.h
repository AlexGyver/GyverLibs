#pragma once
#include <Arduino.h>

/*
	GyverStepper - производительная библиотека для управления шаговыми моторами
	- Поддержка 4х пинового (шаг и полушаг) и STEP-DIR драйверов
	- Автоматическое отключение питания при достижении цели
	- Режимы работы:
		- Вращение с заданной скоростью
		- Следование к позиции с ускорением и ограничением скорости
		- Следование к позиции с заданной скоростью (без ускорения)
	- Быстрый алгоритм управления шагами
	- Два алгоритма плавного движения
		- Мой планировщик обеспечивает максимальную производительность: 
			скорость до 30'000 шагов/сек с ускорением (активен по умолчанию)
		- Модифицированный планировщик из AccelStepper: максимальную плавность и 
			скорость до 7'000 шагов/сек с ускорением (для активации пропиши дефайн SMOOTH_ALGORITHM)
	
	v1.0
		
	Документация: https://alexgyver.ru/gyverstepper/
	Алгоритм из AccelStepper: https://www.airspayce.com/mikem/arduino/AccelStepper/
	AlexGyver, 2020
*/

/*
// Примечание: далее по тексту под "по умолчанию" имеется в виду "даже если не вызывать функцию"

// Создание объекта
// steps - шагов на один оборот вала (для расчётов с градусами)
// step, dir, pin1, pin2, pin3, pin4 - любые GPIO
// en - пин отключения драйвера, любой GPIO
GStepper<STEPPER2WIRE> stepper(steps, step, dir);						// драйвер step-dir
GStepper<STEPPER2WIRE> stepper(steps, step, dir, en);					// драйвер step-dir + пин enable
GStepper<STEPPER4WIRE> stepper(steps, pin1, pin2, pin3, pin4);			// драйвер 4 пин
GStepper<STEPPER4WIRE> stepper(steps, pin1, pin2, pin3, pin4, en);		// драйвер 4 пин + enable
GStepper<STEPPER4WIRE_HALF> stepper(steps, pin1, pin2, pin3, pin4);		// драйвер 4 пин полушаг
GStepper<STEPPER4WIRE_HALF> stepper(steps, pin1, pin2, pin3, pin4, en);	// драйвер 4 пин полушаг + enable

// Здесь происходит движение мотора, вызывать как можно чаще!
// Имеет встроенный таймер
// Возвращает true, если мотор движется к цели или крутится по KEEP_SPEED
bool tick();

// Инвертировать направление мотора - true (по умолч. false)
void reverse(bool dir);

// инвертировать поведение EN пина - true (по умолч. false)
void invertEn(bool rev);

// Установка режима работы, mode:
// FOLLOW_POS - следование к позиции setTarget(...)
// KEEP_SPEED - удержание скорости setSpeed(...)
void setRunMode(GS_runMode mode);

// Установка текущей позиции мотора в шагах и градусах
void setCurrent(long pos);
void setCurrentDeg(float pos);

// Чтение текущей позиции мотора в шагах и градусах
long getCurrent();
float getCurrentDeg();

// установка целевой позиции в шагах и градусах (для режима FOLLOW_POS)
// type - ABSOLUTE или RELATIVE, по умолчанию стоит ABSOLUTE
void setTarget(long pos);
void setTarget(long pos, GS_posType type);
void setTargetDeg(float pos);
void setTargetDeg(float pos, GS_posType type);

// Получение целевой позиции в шагах и градусах
long getTarget();
float getTargetDeg();

// Установка максимальной скорости (по модулю) в шагах/секунду и градусах/секунду (для режима FOLLOW_POS)
// по умолч. 300
void setMaxSpeed(int speed);
void setMaxSpeedDeg(float speed);

// Установка ускорения в шагах и градусах в секунду (для режима FOLLOW_POS).
// При значении 0 ускорение отключается и мотор работает 
// по профилю постоянной максимальной скорости setMaxSpeed().
// По умолч. 300
void setAcceleration(int accel);
void setAccelerationDeg(float accel);

// Автоотключение EN при достижении позиции - true (по умолч. false).
void autoPower(bool mode);

// Плавная остановка с заданным ускорением от текущего положения. 
// Режим будет переключен на FOLLOW_POS
// Установленная максимальная скорость будет изменена!!!
void stop();

// Жёсткая остановка
void brake();

// Жёсткая остановка + сброс позиции в 0 (для концевиков)
void reset();

// Установка целевой скорости в шагах/секунду и градусах/секунду (для режима KEEP_SPEED)
void setSpeed(int speed);
void setSpeedDeg(float speed);

// Получение целевой скорости в шагах/секунду и градусах/секунду (для режима KEEP_SPEED)
int getSpeed();
float getSpeedDeg();

// Включить мотор (пин EN)
void enable();

// Выключить мотор (пин EN)
void disable();

// Возвращает то же самое, что tick, т.е. крутится мотор или нет
bool getState();

// Возвращает минимальный период тика мотора в микросекундах при настроенной setMaxSpeed() скорости.
// Можно использовать для настройки прерываний таймера, в обработчике которого будет лежать tick() (см. пример timerISR)
uint16_t getMinPeriod();

// Текущий период "тика" для отладки и всего такого
uint16_t stepTime;

*/

// Раскомментируй для использования более плавного, но медленного алгоритма
// Также дефайн можно прописать в скетче до подключения библиотеки!!! См. пример smoothAlgorithm
//#define SMOOTH_ALGORITHM

enum GS_driverType {
	STEPPER2WIRE,
	STEPPER4WIRE,
	STEPPER4WIRE_HALF,
};

enum GS_runMode {
	FOLLOW_POS,
	KEEP_SPEED,
};

enum GS_posType {
	ABSOLUTE,
	RELATIVE,
};

#define _sign(x) ((x) >= 0 ? 1 : -1)

template <GS_driverType _DRV>
class GStepper {
public:	
	// конструктор
	GStepper(int stepsPerRev, int8_t pin1, int8_t pin2, int8_t pin3 = -1, int8_t pin4 = -1, int8_t pin5 = -1) : 
	_stepsPerDeg(stepsPerRev / 360.0) {
		if (_DRV == STEPPER2WIRE) {
			configurePin(0, pin1);
			configurePin(1, pin2);
			if (pin3 != -1) {
				_enPin = pin3;
				pinMode(_enPin, OUTPUT);
			}
		} else {
			configurePin(0, pin1);
			configurePin(1, pin2);
			configurePin(2, pin3);
			configurePin(3, pin4);
			if (pin5 != -1) {
				_enPin = pin5;
				pinMode(_enPin, OUTPUT);
			}
		}
		// умолчания
		setMaxSpeed(300);
		setAcceleration(300);
	}
	
	// возвращает true, если мотор всё ещё движется к цели
	bool tick() {
#ifndef SMOOTH_ALGORITHM
		planner();				// планировщик скорости быстрый
#endif		
		if (_workState && micros() - _prevTime >= stepTime) {
			_prevTime = micros();			
			// FOLLOW_POS
			if (!_curMode && _target == _current) {
				_workState = false;
				_accelSpeed = 0;
				if (_autoPower) disable();					
				return false;					
			}
#ifdef SMOOTH_ALGORITHM
			plannerSmooth();	// планировщик скорости	плавный
#endif
			
			// двигаем мотор
			_current += _dir;
			if (_DRV == STEPPER2WIRE) {
				// ~4 us
				setPin(1, (_dir > 0 ? _globDir : !_globDir) );
				setPin(0, 1);	// HIGH
				setPin(0, 0);	// LOW
			} else {
				// ~5.7 us	
				thisStep += (_globDir ? _dir : -_dir);			
				step();
			}			
		}		
		return _workState;
	}
	
	
	// инвертировать направление мотора
	void reverse(bool dir) 			{_globDir = dir;}

	// инвертировать поведение EN пина
	void invertEn(bool dir) 		{_enDir = dir;}

	// установка и чтение текущей позиции в шагах и градусах
	void setCurrent(long pos) 		{_current = pos; _accelSpeed = 0;}
	void setCurrentDeg(float pos) 	{setCurrent((float)pos * _stepsPerDeg);}
	long getCurrent() 				{return _current;}
	float getCurrentDeg() 			{return ((float)_current / _stepsPerDeg);}

	// установка и получение целевой позиции в шагах и градусах
	void setTarget(long pos, GS_posType type = ABSOLUTE) {
		_target = type ? (_target+pos) : pos;		
		if (_target != _current) {
			recalculateSpeed(); 
			_workState = true; 
			if (!_powerState) enable();
		}
	}
	void setTargetDeg(float pos, GS_posType type = ABSOLUTE) {setTarget((float)pos * _stepsPerDeg, type);}
	long getTarget() 				{return _target;}
	float getTargetDeg() 			{return ((float)_target / _stepsPerDeg);}

	// установка максимальной скорости в шагах/секунду и градусах/секунду
	void setMaxSpeed(int speed) {
		_maxSpeed = speed;
		recalculateSpeed();
		
#ifdef SMOOTH_ALGORITHM
		_cmin = 1000000.0 / speed;
		if (_n > 0)	{
			_n = (long)((_accelSpeed * _accelSpeed) / (2.0 * _accel)); // Equation 16
			plannerSmooth();
		}
#else
		// период планировщка в зависимости от макс. скорости
		_plannerPrd = map(speed, 1000, 20000, 15000, 1000);
		_plannerPrd = constrain(_plannerPrd, 15000, 1000);
#endif		
	}
	
	void setMaxSpeedDeg(float speed){setMaxSpeed((float)speed * _stepsPerDeg);}

	// установка ускорения шагах и градусах в секунду
	void setAcceleration(int accel) 		{
		_accel = accel; 
		_accelInv = 0.5f / accel; 
#ifdef SMOOTH_ALGORITHM
		_n = _n * (_accel / accel);
		_c0 = 0.676 * sqrt(2.0 / _accel) * 1000000.0; // Equation 15
		plannerSmooth();
#else		
		_accelTime = accel / 1000000.0f;
#endif
	}
	void setAccelerationDeg(float accel) 	{setAcceleration(accel * _stepsPerDeg);}

	void autoPower(bool mode) 		{_autoPower = mode;}

	// плавная остановка с ускорением
	void stop() {
		_curMode = FOLLOW_POS;
		_accelSpeed = 1000000L / stepTime * _dir;
		setTarget(_current + _accelSpeed * _accelSpeed * _accelInv * _dir);
		setMaxSpeed(abs(_accelSpeed));
#ifdef SMOOTH_ALGORITHM
		_n = _accelSpeed * _accelSpeed * _accelInv;
#endif
	}

	// жёсткая остановка
	void brake() 					{_workState = false; _accelSpeed = 0;}
	void reset()					{brake(); setCurrent(0);}

	// установка и получение целевой скорости в шагах/секунду и градусах/секунду
	void setSpeed(int speed) {
		_speed = speed;
		if (speed != 0) {
			_dir = (speed > 0) ? 1 : -1;
			stepTime = 1000000L / abs(speed);
			_workState = true;
			if (!_powerState) enable();
		} else {
			_workState = false;
		}
	}
	void setSpeedDeg(float speed) 	{setSpeed(_stepsPerDeg * speed);}
	int getSpeed() 					{return (1000000L / stepTime * _dir);}
	float getSpeedDeg() 			{return ((float)getSpeed() / _stepsPerDeg);}

	// установка режима работы
	void setRunMode(GS_runMode mode){_curMode = mode; if (mode == KEEP_SPEED) recalculateSpeed();}
	
	bool getState()					{return _workState;}

	void enable() {
		_powerState = true;
		if (_DRV == STEPPER4WIRE) step();	// подадим прошлый сигнал на мотор, чтобы вал зафиксировался
		if (_enPin != -1) digitalWrite(_enPin, _enDir);
	}
	
	void disable() {
		_powerState = false;
		if (_DRV == STEPPER4WIRE) {
			setPin(0, 0);
			setPin(1, 0);
			setPin(2, 0);
			setPin(3, 0);
		}
		if (_enPin != -1) digitalWrite(_enPin, !_enDir);
	}	
	
	uint16_t getMinPeriod() {
		if (_curMode == KEEP_SPEED) return abs(1000000L / _speed);
		else return 1000000L / _maxSpeed;
	}
	
	uint16_t stepTime = 10000;

private:
	void configurePin(int num, int8_t pin) {
		_port_reg[num] = portOutputRegister(digitalPinToPort(pin));
		_ddr_reg[num] = portModeRegister(digitalPinToPort(pin));
		_bit_mask[num] = digitalPinToBitMask(pin);
		*_ddr_reg[num] |= _bit_mask[num];	// OUTPUT		
	}
	
	void setPin(int num, bool state) {
		if (state) *_port_reg[num] |= _bit_mask[num];
		else *_port_reg[num] &= ~ _bit_mask[num];		
	}
	
	void step() {
		if (_DRV == STEPPER4WIRE) {	
			// 0b11 берёт два бита, т.е. формирует 0 1 2 3 0 1..
			switch (thisStep & 0b11) {			
			case 0: setPin(0, 1); setPin(1, 0); setPin(2, 1); setPin(3, 0); break;	// 1010
			case 1: setPin(0, 0); setPin(1, 1); setPin(2, 1); setPin(3, 0); break;	// 0110
			case 2: setPin(0, 0); setPin(1, 1); setPin(2, 0); setPin(3, 1); break;	// 0101
			case 3: setPin(0, 1); setPin(1, 0); setPin(2, 0); setPin(3, 1); break;	// 1001
			}			
		} else if (_DRV == STEPPER4WIRE_HALF) {
			// 0b111 берёт три бита, т.е. формирует 0 1 2 4 5 6 7 0 1 2..
			switch (thisStep & 0b111) {
			case 0: setPin(0, 1); setPin(1, 0); setPin(2, 0); setPin(3, 0); break;	// 1000
			case 1: setPin(0, 1); setPin(1, 0); setPin(2, 1); setPin(3, 0); break;	// 1010
			case 2: setPin(0, 0); setPin(1, 0); setPin(2, 1); setPin(3, 0); break;	// 0010
			case 3: setPin(0, 0); setPin(1, 1); setPin(2, 1); setPin(3, 0); break;	// 0110
			case 4: setPin(0, 0); setPin(1, 1); setPin(2, 0); setPin(3, 0); break;	// 0100
			case 5: setPin(0, 0); setPin(1, 1); setPin(2, 0); setPin(3, 1); break;	// 0101
			case 6: setPin(0, 0); setPin(1, 0); setPin(2, 0); setPin(3, 1); break;	// 0001
			case 7: setPin(0, 1); setPin(1, 0); setPin(2, 0); setPin(3, 1); break;	// 1001
			}
		}
	}
	
	void recalculateSpeed() {
		if (!_curMode && _accel == 0) {
			stepTime = 1000000L / _maxSpeed;
			_dir = (_target > _current) ? 1 : -1;
		}
	}
	
#ifdef SMOOTH_ALGORITHM
	// планировщик скорости из AccelStepper
	void plannerSmooth() {
		if (!_curMode && _accel != 0) {
			long err = _target - _current;
			long stepsToStop = _accelSpeed * _accelSpeed * _accelInv;

			if (err == 0 && stepsToStop <= 1) {
				stepTime = 0;
				_accelSpeed = 0.0;
				_n = 0;
				return;
			}

			if (err > 0) {
				if (_n > 0) {
					if ((stepsToStop >= err) || _dir == -1)
					_n = -stepsToStop;
				} else if (_n < 0) {
					if ((stepsToStop < err) && _dir == 1)
					_n = -_n;
				}
			} else if (err < 0) {
				if (_n > 0) {
					if ((stepsToStop >= -err) || _dir == 1)
					_n = -stepsToStop;
				} else if (_n < 0) {
					if ((stepsToStop < -err) && _dir == -1)
					_n = -_n;
				}
			}

			if (_n == 0) {
				_cn = _c0;
				_dir = _sign(err);;
			} else {
				_cn = _cn - ((2.0 * _cn) / ((4.0 * _n) + 1));
				_cn = max(_cn, _cmin); 
			}
			_n++;
			stepTime = _cn;
			_accelSpeed = 1000000.0 / _cn;
			if (_dir == -1) _accelSpeed = -_accelSpeed;
		}
	}

	long _n = 0;
	float _c0 = 0.0;
	float _cn = 0.0;
	float _cmin = 1.0;

#else
	// планировщик скорости мой
	void planner() {		
		if (_workState && !_curMode && _accel != 0) {			
			if (micros() - _plannerTime >= _plannerPrd) {
				_plannerTime = micros();
				// ~110 us				
				long err = _target - _current;											// "ошибка"
				bool thisDir = ( _accelSpeed * _accelSpeed * _accelInv >= abs(err) );	// пора тормозить
				_accelSpeed += ( _accelTime * _plannerPrd * (thisDir ? -_sign(_accelSpeed) : _sign(err)) );	// разгон/торможение
				_accelSpeed = constrain(_accelSpeed, -_maxSpeed, _maxSpeed);			// ограничение
				
				if (abs(_accelSpeed) > 50) stepTime = abs(1000000.0 / _accelSpeed);		// ограничение на мин. скорость
				else stepTime = 20000;													// 20000 == 1000000/50
				_dir = _sign(_accelSpeed);												// направление для шагов
			}
		}
	}
	
	uint32_t _plannerTime = 0;
	int _plannerPrd = 15000;	
	float _accelTime = 0;
#endif

	const float _stepsPerDeg;
	int8_t _enPin = -1;
	uint32_t _prevTime = 0;			
	float _accelSpeed = 0;
	int32_t _current = 0;
	int32_t _target = 0;

	int8_t thisStep = 0;
	int8_t _dir = 1;
	bool _globDir = false;
	bool _enDir = false;
	bool _powerState = false;
	bool _workState = false;
	bool _autoPower = false;

	int _maxSpeed = 300;
	int _speed = 0;
	int _accel = 0;	
	float _accelInv = 0;

	GS_runMode _curMode = FOLLOW_POS;

	volatile uint8_t *_port_reg[_DRV == 0 ? 2 : 4];
	volatile uint8_t *_ddr_reg[_DRV == 0 ? 2 : 4];
	volatile uint8_t _bit_mask[_DRV == 0 ? 2 : 4];
};