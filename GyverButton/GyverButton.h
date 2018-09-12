#ifndef GyverButton_h
#define GyverButton_h
#include <Arduino.h>
#define LIBRARY_VERSION	2.5

#define HIGH_PULL 0		// кнопка подключена к GND, пин подтянут к VCC (по умолчанию)
#define LOW_PULL 1		// кнопка подключена к VCC, пин подтянут к GND
#define NORM_OPEN 0		// кнопка по умолчанию разомкнута
#define NORM_CLOSE 1	// кнопка по умолчанию замкнута
#define MANUAL 0		// нужно вызывать функцию tick() вручную
#define AUTO 1			// tick() входит во все остальные функции и опрашивается сама!

/*
Текущая версия: 2.5 от 12.09.2018
GyverButton - библиотека для полной отработки нажатия кнопки. Возможности:
Возможность опрашивать не кнопку, а напрямую давать величину
Опрос кнопки с программным антидребезгом контактов
Отработка нажатия, удерживания отпускания кнопки
Отработка одиночного, двойного и тройного нажатия (вынесено отдельно)
Отработка любого количества нажатий кнопки (функция возвращает число нажатий)
Отработка нажатия и удержания кнопки
Настраиваемый таймаут повторного нажатия/удержания
Функция изменения значения переменной с заданным шагом и заданным интервалом по времени
Примеры использования в папке examples, показывают все возможности библиотеки
Отличия от oneBtton и подобных библиотек: методы библиотеки не создают новые функции, что упрощает применение в сотни раз
*/

class GButton
{
  public:
    GButton(uint8_t);					// класс кнопки, принимает пин
	GButton(uint8_t, boolean, boolean);	// класс кнопки, принимает пин, тип (HIGH_PULL / LOW_PULL) и направление (NORM_OPEN / NORM_CLOSE)
	void setDebounce(uint16_t);			// установка времени антидребезга (умолч. 80 мс)
	void setTimeout(uint16_t);			// установка таймаута удержания (умолч. 500 мс)
	void setStepTimeout(uint16_t);		// установка таймаута между инкрементами (умолч. 400 мс)
	void tick();						// опрос кнопки
	void tick(boolean);					// опрос внешнего значения (для матричных и резистивных клавиатур)
	void setType(boolean);				// установка типа кнопки (HIGH_PULL - подтянута к питанию, LOW_PULL - к gnd)
	void setDirection(boolean);			// установка направления (разомкнута/замкнута по умолчанию - NORM_OPEN, NORM_CLOSE)
	void setTickMode(boolean);			// ручной или автоматический опрос кнопки функцией tick()
	
	boolean isPress();		// возвращает true при нажатии на кнопку. Сбрасывается после вызова
	boolean isRelease();	// возвращает true при отпускании кнопки. Сбрасывается после вызова
	boolean isClick();		// возвращает true при клике. Сбрасывается после вызова
    boolean isHolded();		// возвращает true при удержании дольше timeout. Сбрасывается после вызова
	boolean isHold();		// возвращает true при нажатой кнопке, не сбрасывается
	boolean state();		// возвращает состояние кнопки
	
	boolean hasClicks();	// проверка на наличие кликов. Сбрасывается после вызова
	uint8_t getClicks();	// вернуть количество кликов
	
	boolean isSingle();		// возвращает true при одиночном клике. Сбрасывается после вызова
	boolean isDouble();		// возвращает true при двойном клике. Сбрасывается после вызова
	boolean isTriple();		// возвращает true при тройном клике. Сбрасывается после вызова
	
	boolean isStep();		// возвращает true по таймеру setStepTimeout, смотри пример
	
  private:
    uint8_t _PIN;
	uint16_t _debounce;
	uint16_t _timeout;
	uint16_t _step_timeout;
	uint8_t btn_counter, last_counter;
	boolean btn_state, btn_flag, hold_flag, counter_flag;
	uint32_t btn_timer;	
	boolean isHolded_f, isRelease_f, isPress_f, step_flag, oneClick_f, isOne_f;	
	boolean _inv_state = false;
	boolean _mode = false;
	boolean _type = false;
	boolean _tickMode = false;
};
 
#endif