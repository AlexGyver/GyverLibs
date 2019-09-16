#ifndef GyverEncoder_h
#define GyverEncoder_h
#include <Arduino.h>

/*	
	GyverEncoder - библиотека для отработки энкодера. Возможности:
	- Отработка поворота с антидребезгом
	- Отработка нажатия кнопки с антидребезгом
	- Отработка нажатия и удержания кнопки
	- Отработка "нажатого поворота"
	- Работа с двумя типами экнодеров
	- Отработка "быстрого поворота"
	- Версия 3+ более оптимальная и быстрая
	
	Текущая версия: 3.6 от 16.09.2019
	- Возвращены дефайны настроек
*/

// настройка антидребезга энкодера, кнопки и таймаута удержания
const byte DEBOUNCE_TURN = 1;
const int DEBOUNCE_BUTTON = 80;
const int HOLD_TIMEOUT = 700;

#pragma pack(push,1)
typedef struct
{	
	bool SW_state: 1;
	bool hold_flag: 1;
	bool butt_flag: 1;
	bool turn_flag: 1;
	bool isTurn_f: 1;
	bool isPress_f: 1;
	bool isRelease_f: 1;
	bool isHolded_f: 1;
	bool isFastR_f: 1;
	bool isFastL_f: 1;
	bool enc_tick_mode: 1;
	bool enc_type: 1;
	bool use_button : 1;

} GyverEncoderFlags;
#pragma pack(pop)

#define TYPE1 0		// полушаговый энкодер
#define TYPE2 1		// полношаговый
#define NORM 0		// направление вращения обычное
#define REVERSE 1	// обратное
#define MANUAL 0		// нужно вызывать функцию tick() вручную
#define AUTO 1		// tick() входит во все остальные функции и опрашивается сама!

class Encoder
{
public:
	Encoder(uint8_t clk, uint8_t dt);							// CLK, DT	
	Encoder(uint8_t clk, uint8_t dt, uint8_t sw);				// CLK, DT, SW
	Encoder(uint8_t clk, uint8_t dt, uint8_t sw, uint8_t type); // CLK, DT, SW, тип (TYPE1 / TYPE2) TYPE1 одношаговый, TYPE2 двухшаговый. Если ваш энкодер работает странно, смените тип
	
	void tick();							// опрос энкодера, нужно вызывать постоянно или в прерывании
	void setType(uint8_t type);				// TYPE1 / TYPE2 - тип энкодера TYPE1 одношаговый, TYPE2 двухшаговый. Если ваш энкодер работает странно, смените тип
	void setTickMode(uint8_t tickMode); 	// MANUAL / AUTO - ручной или автоматический опрос энкодера функцией tick(). (по умолчанию ручной)
	void setDirection(uint8_t direction);	// NORM / REVERSE - направление вращения энкодера
	void setFastTimeout(int timeout);		// установка таймаута быстрого поворота
	
	boolean isTurn();						// возвращает true при любом повороте, сама сбрасывается в false
	boolean isRight();						// возвращает true при повороте направо, сама сбрасывается в false
	boolean isLeft();						// возвращает true при повороте налево, сама сбрасывается в false
	boolean isRightH();						// возвращает true при удержании кнопки и повороте направо, сама сбрасывается в false
	boolean isLeftH();						// возвращает true при удержании кнопки и повороте налево, сама сбрасывается в false
	boolean isFastR();						// возвращает true при быстром повороте
	boolean isFastL();						// возвращает true при быстром повороте
	
	boolean isPress();						// возвращает true при нажатии кнопки, сама сбрасывается в false
	boolean isRelease();					// возвращает true при отпускании кнопки, сама сбрасывается в false
	boolean isClick();						// возвращает true при нажатии и отпускании кнопки, сама сбрасывается в false
	boolean isHolded();						// возвращает true при удержании кнопки, сама сбрасывается в false
	boolean isHold();						// возвращает true при удержании кнопки, НЕ СБРАСЫВАЕТСЯ
	
	int8_t fast_timeout = 50;				// таймаут быстрого поворота
	
private:
	void init();
	GyverEncoderFlags flags;
	byte curState, prevState;
	byte encState;	// 0 не крутился, 1 лево, 2 право, 3 лево нажат, 4 право нажат
	uint32_t debounce_timer = 0, fast_timer;
	byte _CLK = 0, _DT = 0, _SW = 0;
	
};

#endif