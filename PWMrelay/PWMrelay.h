#ifndef PWMrelay_h
#define PWMrelay_h
#include <Arduino.h>
/*
	PWMrelay - библиотека для генерации низкочастотного ШИМ сигнала для реле (для ПИД регуляторов и проч.)
	- Настройка периода ШИМ
	- Настройка сигнала 0-255 (8 бит)
	- Поддержка реле низкого и высокого уровня
	- Неблокирующий вызов, не использует таймеры (кроме системного), работает на millis()
	
	v1.1 - убрана реализация из .h
*/

class PWMrelay {
public:
	PWMrelay(byte pin, bool dir, int period);	// пин, уровень реле HIGH/LOW, период
	void tick();					// тик, вызывать как можно чаще, сам управляет реле
	void setPWM(byte duty);			// установить величину ШИМ, 0-255. При значении 0 и 255 тик неактивен!
	byte getPWM();					// возвращает величину ШИМ
	void setPeriod(int period);		// установить период ШИМ в миллисек. (по умолч. 1000мс == 1с)
	int getPeriod();				// получить период
	void setLevel(bool level);		// установить установить уровень реле (HIGH/LOW)
	
private:
	bool _flag = false;
	bool _dir = false;
	byte _pin = 0;
	byte _duty = 0;
	int _period = 1000;
	int _activePeriod = 0;
	uint32_t _tmr = 0;
};
#endif