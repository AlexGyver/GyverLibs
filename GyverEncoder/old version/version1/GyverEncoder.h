#ifndef GyverRC_h
#define GyverRC_h
#include <Arduino.h>

/*
Текущая версия: 1.1 от 29.03.2018
GyverEncoder - библиотека для отработки энкодера. Возможности:
Отработка поворота
Отработка нажатия кнопки с антидребезгом
Отработка нажатия и удержания кнопки
Отработка "нажатого поворота" - такого вы нигде не найдёте. Расширяет возможности энкодера ровно в 2 раза
Имеет встроенный счётчик для поворота, всё можно настроить
Настраиваемые пределы изменяемой величины, а также шаг изменения
Пример использования в папке examples, показывает все возможности библиотеки
*/

class Encoder
{
  public:
    Encoder(uint8_t, uint8_t, uint8_t);
	
	void setCounters(int norm, int hold);
	void setCounterNorm(int norm);
	void setCounterHold(int hold);
	
	void setSteps(int norm_step, int hold_step);
	void setStepNorm(int norm_step);	
	void setStepHold(int hold_step);	
	
	void setLimitsNorm(int normMin, int normMax);
	void setLimitsHold(int holdMin, int holdMax);
	
	void invert();	
	void tick();
	void setType(boolean type);
	
	boolean isTurn();	
	
	boolean isRight();
	boolean isLeft();	
	boolean isRightH();
	boolean isLeftH();
	
	boolean isPress();
	boolean isRelease();
    boolean isHolded();
	boolean isHold();
	
	int normCount, holdCount;
	
  private:
    byte _CLK, _DT, _SW;
	
    boolean DT_now, DT_last, SW_state, hold_flag, butt_flag, turn_flag;
	boolean isRight_f, isLeft_f, isRightH_f, isLeftH_f, isTurn_f;
	boolean isPress_f, isRelease_f, isHolded_f, isHold_f;
	boolean _type, _new_step = true;
	
	int normCountMin, normCountMax, holdCountMin, holdCountMax;
	uint8_t normCount_step, holdCount_step;
	
	unsigned long debounce_timer;
};
 
#endif