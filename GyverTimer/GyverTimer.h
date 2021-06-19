#ifndef GyverTimer_h
#define GyverTimer_h
#include <Arduino.h>

/*
	GTimer - полноценный таймер на базе системных millis() / micros()
	- Миллисекундный и микросекундный таймер
	- Два режима работы:
		- Режим интервала: таймер "срабатывает" каждый заданный интервал времени
		- Режим таймаута: таймер "срабатывает" один раз по истечении времени (до следующего перезапуска)
	- Служебные функции:
		- Старт
		- Стоп
		- Сброс
		- Продолжить
	
	Версии:
	- 2.0 - улучшенный алгоритм работы таймера
		- Кратные интервалы
		- Защита от пропусков
		- Защита от переполнения millis()
		- Убраны дефайны
	- 2.1 - возвращены дефайны
	- 2.2 - улучшена стабильность
	
	- 3.0
		- Логика работы разделена на интервал и таймаут
		- Добавлен общий класс GTimer (для миллисекундного и микросекундного таймера)
		- Добавлена возможность остановить и продолжить счёт таймера
	- 3.2
		- Добавлен isEnabled
		- Возможность не запускать таймер при создании
	- 3.3
		- Добавлен режим "сразу готов" (isReady == true сразу после запуска)
		- Добавлен getInterval()
		- Экономия памяти за счет перехода с boolean на биты
*/

enum timerType {
	US,
	MS,
};

// ============== GTimer (микросекундный и миллисекундный таймер) ================
class GTimer {
  public:
	GTimer(const timerType &type = MS, const uint32_t &interval = 0, const boolean &readyOnStart = false);	// объявление таймера с указанием типа и интервала (таймер не запущен, если не указывать)
	void setInterval(const uint32_t &interval);	// установка интервала работы таймера (также запустит и сбросит таймер) - режим интервала
	void setTimeout(const uint32_t &timeout);	// установка таймаута работы таймера (также запустит и сбросит таймер) - режим таймаута
	void setReadyOnStart(const boolean &readyOnStart);	// установка нулевой готовности таймера (поменяет на ИНТЕРВАЛ, запустит и сбросит таймер)
	uint32_t getInterval();				// возвращает значение текущего интервала/таймаунта
	boolean isReady();					// возвращает true, когда пришло время
	boolean isEnabled();				// вернуть состояние таймера (остановлен/запущен)
	void reset();						// сброс таймера на установленный период работы
	void start();						// запустить/перезапустить (со сбросом счёта)
	void stop();						// остановить таймер (без сброса счёта)	
	void resume();						// продолжить (без сброса счёта)	
	
	// служебное
	void setMode(const boolean &mode);	// установка режима работы вручную: AUTO или MANUAL (TIMER_INTERVAL / TIMER_TIMEOUT)
	
  private:
	uint32_t _timer = 0;
	uint32_t _interval = 0;
	uint32_t _resumeBuffer = 0;
	// boolean _mode = true;				// AUTO или MANUAL
	// boolean _state = false;				// запущен или остановлен
	// boolean _type = true;				// MS или US
	// boolean _readyOnStart = false;		// будет ли "готов" сразу же после запуска или нет
	// boolean _justStarted = false;		// триггер на "готовность" сразу после старта
	byte flags = 0b00010100;			// один байт для всех "булек". биты начиная со старшего отвечают за (X - не используется):
										// значения: [X X X _justStarted _readyOnStart _type _state _mode>]
										// индексы:  [7 6 5      4             3         2      1      0 >]
};

#define MANUAL 			0
#define AUTO 			1
#define TIMER_TIMEOUT 	0
#define TIMER_INTERVAL 	1

#define GTBIT_MODE 			0
#define GTBIT_STATE 		1
#define GTBIT_TYPE 			2
#define GTBIT_READYONSTART 	3
#define GTBIT_JUSTSTARTED 	4

// Макросы для работы с флагами
#define flagRead(flag) bitRead(flags, flag)				// получить значение флага
#define flagSet(flag) bitSet(flags, flag)				// установить флаг в 1
#define flagClear(flag) bitClear(flags, flag)			// установить флаг в 0
#define flagWrite(flag, val) bitWrite(flags, flag, val)	// установить флаг в значение val



// ================================================================================
// ================== УСТАРЕЛО (но работает, для совместимости) ===================
// ================================================================================
class GTimer_ms {
  public:
	GTimer_ms();							// объявление таймера (МИЛЛИСЕКУНДНЫЙ)
	GTimer_ms(uint32_t interval);			// объявление таймера с указанием интервала
	void setInterval(uint32_t interval);	// установка интервала работы таймера (также запустит таймер и сбросит период) - режим авто
	void setTimeout(uint32_t interval);		// установка таймаута работы таймера (также запустит таймер и сбросит период) - режим ручной
	void setMode(uint8_t mode);				// установка типа работы: AUTO или MANUAL (MANUAL нужно вручную сбрасывать reset или setInterval/setTimeout)
	boolean isReady();						// возвращает true, когда пришло время
	void reset();							// ручной сброс таймера на установленный период работы
	void stop();							// остановить таймер (без сброса счёта)
	void start();							// продолжить (без сброса счёта)
	
  private:
	uint32_t _timer = 0;
	uint32_t _interval = 1000;
	boolean _mode = true;
	boolean _state = true;
};

class GTimer_us {
  public:
	GTimer_us();							// объявление таймера (МИКРОСЕКУНДНЫЙ)
	GTimer_us(uint32_t interval);			// объявление таймера с указанием интервала
	void setInterval(uint32_t interval);	// установка интервала работы таймера (также запустит таймер и сбросит период) - режим авто
	void setTimeout(uint32_t interval);		// установка таймаута работы таймера (также запустит таймер и сбросит период) - режим ручной
	void setMode(uint8_t mode);				// установка типа работы: AUTO или MANUAL (MANUAL нужно вручную сбрасывать reset или setInterval/setTimeout)
	boolean isReady();						// возвращает true, когда пришло время
	void reset();							// ручной сброс таймера на установленный интервал
	void stop();							// остановить таймер (без сброса счёта)
	void start();							// продолжить (без сброса счёта)
	
  private:
	uint32_t _timer = 0;
	uint32_t _interval = 1000;
	boolean _mode = true;
	boolean _state = true;
};
#endif