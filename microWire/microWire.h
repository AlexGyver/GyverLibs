/*
	Лёгкая библиотека с минимальным набором инстурментов для работы с модулями по шине I2C
	Не является полноценным аналогом библиотеки Wire!!!
	Не все библиотеки на Wire смогут работать с microWire. microWire создана для работы с устройствами в ручном режимн
	Версия 1.0 by Egor 'Nich1con' Zaharov
*/

#ifndef microWire_h
#define microWire_h
#include <Arduino.h>

class I2C {
	public:
		void begin(int baudrate = 400);
		void setClock(uint32_t clock);
		void beginTransmission(void);
		void beginTransmission(uint8_t addr);
		void endTransmission(void);
		void write(uint8_t data);
		void requestFrom(uint8_t addr);
		uint8_t receive_ack(void);
		uint8_t receive_nack(void);
	private:
	
};
extern I2C Wire;
#endif