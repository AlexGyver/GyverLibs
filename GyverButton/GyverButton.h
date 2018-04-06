#ifndef GyverButton_h
#define GyverButton_h
#include <Arduino.h>

/*
Текущая версия: 1.1 от 29.03.2018
GyverButton - библиотека для полной отработки нажатия кнопки. Возможности:
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
    GButton(uint8_t BUTT);	
	void setDebounce(uint8_t debounce);
	void setTimeout(uint16_t timeout);	
	void tick();
	
	boolean isPress();
	boolean isRelease();
    boolean isHolded();
	boolean isHold();
	
	boolean hasClicks();
	uint8_t getClicks();
	
	boolean isSingle();
	boolean isDouble();
	boolean isTriple();
	
	boolean isIncr();
	void setIncrStep(int16_t incr_step);
	void setIncrTimeout(uint16_t incr_timeout);
	int16_t getIncr(int16_t incr_value);
	
	int16_t step;
	
  private:
    uint8_t _BUTT;
	uint8_t _debounce;
	uint16_t _timeout;
	uint8_t btn_counter, last_counter;
	boolean btn_state, btn_flag, hold_flag, counter_flag;
	uint32_t btn_timer, incr_timer;	
	boolean isHold_f, isHolded_f, isRelease_f, isPress_f, incr_flag;	
	uint16_t _incr_timeout;
};
 
#endif