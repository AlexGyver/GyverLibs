#ifndef GyverButton_h
#define GyverButton_h
#include <Arduino.h>

/*
Текущая версия: 2.2 от 10.09.2018
GyverButton - библиотека для полной отработки нажатия кнопки. Возможности:
Возможность опрашивать не кнопку, а напрямую давать величину
Опрос кнопки с программным антидребезгом контактов
Отработка нажатия, удерживания отпускания кнопки
Отработка одиночного, двойного и тройного нажатия (вынесено отдельно)
Отработка любого количества нажатий кнопки (функция возвращает число нажатий)
Отработка нажатия и удержания кнопки
Настраиваемый таймаут повторного нажатия/удержания
Функция изменения значения переменной с заданным шагом и заданным интервалом по времени
Пример использования в папке examples, показывает все возможности библиотеки
Отличия от oneBtton и подобных библиотек: методы библиотеки не создают новые функции, что упрощает применение в сотни раз
*/

class GButton
{
  public:
    GButton(uint8_t);	
	void setDebounce(uint8_t);
	void setTimeout(uint16_t);	
	void setStepTimeout(uint16_t);
	void tick();
	void tick(boolean);
	void inverse(boolean);
	
	boolean isPress();
	boolean isRelease();
	boolean isClick();
    boolean isHolded();
	boolean isHold();
	boolean state();
	
	boolean hasClicks();
	uint8_t getClicks();
	
	boolean isSingle();
	boolean isDouble();
	boolean isTriple();
	
	boolean isStep();
	
  private:
    uint8_t _BUTT;
	uint8_t _debounce;
	uint16_t _timeout;
	uint8_t btn_counter, last_counter;
	boolean btn_state, btn_flag, hold_flag, counter_flag;
	uint32_t btn_timer;	
	boolean _state, isHolded_f, isRelease_f, isPress_f, step_flag, oneClick_f, isOne_f;	
	uint16_t _step_timeout;
	boolean _inv_state = false;
	boolean _mode = false;
};
 
#endif