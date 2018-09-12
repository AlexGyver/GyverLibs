#ifndef GyverRC_h
#define GyverRC_h
#include <Arduino.h>
#define LIBRARY_VERSION	2.4

/*
Текущая версия: 2.4 от 12.09.2018
GyverEncoder - библиотека для отработки энкодера. Возможности:
Отработка поворота с антидребезгом
Отработка нажатия кнопки с антидребезгом
Отработка нажатия и удержания кнопки
Отработка "нажатого поворота"
Пример использования в папке examples, показывает все возможности библиотеки
*/

#define debounce_turn 30
#define debounce_button 100
#define hold_timer 800

#define TYPE1 0			// полушаговый энкодер
#define TYPE2 1			// полношаговый
#define MANUAL 0		// нужно вызывать функцию tick() вручную
#define AUTO 1			// tick() входит во все остальные функции и опрашивается сама!

class Encoder
{
  public:
    Encoder(uint8_t, uint8_t, uint8_t);				// CLK, DT, SW
	Encoder(uint8_t, uint8_t, uint8_t, boolean);	// CLK, DT, SW, тип (TYPE1 / TYPE2)
		
	void tick();				// опрос энкодера, нужно вызывать постоянно или в прерывании
	void setType(boolean);		// тип энкодера TYPE1 одношаговый, TYPE2 двухшаговый. Если ваш энкодер работает странно, смените тип
	void setTickMode(boolean); 	// ручной (MANUAL) или автоматический (AUTO) опрос энкодера функцией tick(). (по умолчанию ручной)
	void invert();				// инвертировать направление. Повторный вызов функции снова инвертирует!
	
	boolean isTurn();			// возвращает true при любом повороте, сама сбрасывается в false
	boolean isRight();			// возвращает true при повороте направо, сама сбрасывается в false
	boolean isLeft();			// возвращает true при повороте налево, сама сбрасывается в false
	boolean isRightH();			// возвращает true при удержании кнопки и повороте направо, сама сбрасывается в false
	boolean isLeftH();			// возвращает true при удержании кнопки и повороте налево, сама сбрасывается в false
	
	boolean isPress();			// возвращает true при нажатии кнопки, сама сбрасывается в false
	boolean isRelease();		// возвращает true при отпускании кнопки, сама сбрасывается в false
    boolean isHolded();			// возвращает true при удержании кнопки, сама сбрасывается в false
	boolean isHold();			// возвращает true при удержании кнопки, НЕ СБРАСЫВАЕТСЯ
	
  private:
    byte _CLK, _DT, _SW;	
    boolean DT_now, DT_last, SW_state, hold_flag, butt_flag, turn_flag;
	boolean isRight_f, isLeft_f, isRightH_f, isLeftH_f, isTurn_f;
	boolean isPress_f, isRelease_f, isHolded_f, isHold_f;
	boolean _type, _new_step = true, _tickMode = false;
	unsigned long debounce_timer;
};
 
#endif