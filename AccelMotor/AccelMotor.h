#ifndef AccelMotor_h
#define AccelMotor_h
#include <Arduino.h>
#include <GyverMotor.h>

// v1.1
// v1.2 - совместимость с есп
// v1.3 - небольшие улучшения и фиксы

/*
	Библиотека для расширенного управления и стабилизации мотора с энкодером
	Документация: https://alexgyver.ru/accelmotor/
	- Наследует все фишки из библиотеки GyverMotor (поддержка разных драйверов и режимов)
	- Режим поддержания скорости с обратной связью
	- Режим поворота на заданный угол с обратной связью
	- Настраиваемые коэффициенты PID регулятора
	- Ограничение ускорения и скорости
	- Библиотека принимает любой тип обратной связи: энкодер, потенциометр, и т.д.
	- Поддержка мотор-редукторов, настройка передаточного отношения энкодера
	- Регулятор учитывает "мёртвую зону" мотора
	- Все функции работают в градусах и "тиках" энкодера
*/


enum AM_runMode {	
	ACCEL_POS,
	PID_POS,
	ACCEL_SPEED,
	PID_SPEED,
	IDLE_RUN,
};

class AccelMotor : public GMotor {
public:
	using GMotor::GMotor;
	
	// управляет мотором. Вызывать как можно чаще (внутри таймер с периодом dt)
	// принимает текущее положение вала мотора (по счёту энкодера)
	// возвращает true если мотор всё ещё движется к цели
	bool tick(long pos);							
	
	// установка передаточного отношения редуктора и энкодера
	// пример: если редуктор 1:30 - передаём в функцию 30
	// пример: если редуктор 1:30 и энкодер на 12 тиков - передаём 30*12
	void setRatio(float ratio);	

	// установка периода регулятора (рекомендуется 2-50 миллисекунд)
	void setDt(int dt);
	
	// установка и получение текущей позиции в тиках энкодера и градусах.
	// setCurrent(pos) равносильна вызову tick(pos) и в принципе не нужна!
	void setCurrent(long pos);
	long getCurrent();
	long getCurrentDeg();
	
	// установка и получение целевой позиции в тиках энкодера и градусах
	void setTarget(long pos);
	void setTargetDeg(long pos);
	long getTarget();
	long getTargetDeg();
	
	// установка максимальной скорости в тиках энкодера/секунду и градусах/секунду
	void setMaxSpeed(int speed);
	void setMaxSpeedDeg(int speed);
	
	// установка ускорения тиках энкодера и градусах в секунду
	void setAcceleration(int accel);
	void setAccelerationDeg(int accel);
	
	// установка и получение целевой скорости в тиках энкодера/секунду и градусах/секунду
	void setTargetSpeed(int speed);
	void setTargetSpeedDeg(int speed);
	int getTargetSpeed();
	int getTargetSpeedDeg();
	
	// получить текущую скорость в тиках энкодера/секунду и градусах/секунду
	int getSpeed();
	int getSpeedDeg();	
	
	// получить текущий ШИМ сигнал (float из ПИД регулятора)
	float getDuty();
	
	// ручная установка режима работы
	// IDLE_RUN - tick() не управляет мотором. Может использоваться для отладки
	// ACCEL_POS - tick() работает в режиме плавного следования к целевому углу
	// PID_POS - tick() работает в режиме резкого следования к целевому углу
	// ACCEL_SPEED - tick() работает в режиме плавного поддержания скорости (с заданным ускорением)
	// PID_SPEED - tick() работает в режиме поддержания скорости по ПИД регулятору
	void setRunMode(AM_runMode mode);
	
	// возвращает true, если вал мотора заблокирован, а сигнал подаётся
	bool isBlocked();
	
	// коэффициенты ПИД регулятора
	// пропорциональный - от него зависит агрессивность управления, нужно увеличивать kp
	// при увеличении нагрузки на вал, чтобы регулятор подавал больший управляющий ШИМ сигнал
	float kp = 2.0;		// (знач. по умолчанию)
	
	// интегральный - позволяет нивелировать ошибку со временем, имеет накопительный эффект
	float ki = 0.9;		// (знач. по умолчанию)
	
	// дифференциальный. Позволяет чуть сгладить рывки, но при большом значении
	// сам становится причиной рывков и раскачки системы!
	float kd = 0.1;		// (знач. по умолчанию)
	
	// установить зону остановки мотора для режима стабилизации позиции (по умолч. 8)
	void setStopZone(int zone);
	
	// установить пределы шагов/градусов, вне которых мотор будет жёстко отключен для безопасности. Если по нулям, ограничения нет (по умолч.)
	void setRange(long min, long max);
	void setRangeDeg(long min, long max);
	
	long controlPos = 0;	// для отладки
private:
	int filter(int newVal);
	int _buf[3];
	byte _count = 0;
	float _middle_f = 0;
	long _min = 0, _max = 0;
	float _lastSpeed = 0;	
	void PIDcontrol(long target, long current, bool cutoff);
	float integral = 0;
	int _dt = 20;
	float _dts = 0.02;
	long _lastPos = 0, _currentPos = 0, _targetPos = 0;
	int _curSpeed = 0;
	int _maxSpeed = 300, _targetSpeed = 0;
	float _ratio = 1;
	uint32_t _tmr2 = 0;
	int _accel = 300;
	float _dutyF = 0;	
	float controlSpeed = 0;
	int _stopzone = 8;
	long _prevInput = 0;
	AM_runMode _runMode = IDLE_RUN;
};

/*
	======= НАСЛЕДУЕТСЯ ИЗ GYVERMOTOR =======
	GMotor(driverType type, int8_t param1 = NC, int8_t param2 = NC, int8_t param3 = NC, int8_t param4 = NC);
	// три варианта создания объекта в зависимости от драйвера:
	// GMotor motor(DRIVER2WIRE, dig_pin, PWM_pin, (LOW/HIGH) )
	// GMotor motor(DRIVER3WIRE, dig_pin_A, dig_pin_B, PWM_pin, (LOW/HIGH) )
	// GMotor motor(RELAY2WIRE, dig_pin_A, dig_pin_B, (LOW/HIGH) )
	
	// установка скорости 0-255 (8 бит) и 0-1023 (10 бит)
	void setSpeed(int16_t duty);			
	
	// сменить режим работы мотора:	
	// FORWARD - вперёд
	// BACKWARD - назад
	// STOP - остановить
	// BRAKE - активное торможение
	void setMode(workMode mode);
	
	// направление вращения	
	// NORM - обычное
	// REVERSE - обратное
	void setDirection(dir direction);
	
	// установить минимальную скважность (при которой мотор начинает крутиться)
	void setMinDuty(int duty);
	
	// установить выход в 8 бит
	void set8bitMode();		

	// установить выход в 10 бит
	void set10bitMode();					
	
	// установить deadtime (в микросекундах). По умолч 0
	void setDeadtime(uint16_t deadtime);	
	
	// установить уровень драйвера (по умолч. HIGH)
	void setLevel(int8_t level);			
	
	// плавное изменение к указанной скорости (к величине ШИМ)
	void smoothTick(int16_t duty);
	
	// скорость изменения скорости
	void setSmoothSpeed(uint8_t speed);
	
	// дать прямую команду мотору (без смены режима)
	void run(workMode mode, int16_t duty);	
	
	// возвращает -1 при вращении BACKWARD, 1 при FORWARD и 0 при остановке и торможении
	int getState();
*/
#endif