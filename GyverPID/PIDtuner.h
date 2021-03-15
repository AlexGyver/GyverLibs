#ifndef PIDtuner_h
#define PIDtuner_h
/*
	Автоматический калибровщик коэффициентов ПИД регулятора, метод "реле" http://auto-controls.blogspot.com/2009/10/pid-controllers-auto-tuning-relay.html
	Данный тюнер лучше настраивает коэффициенты для удержания величины и парирования внешних возмущений
	Версия 1.0
	
	=== Как это работает? ===
	1. Тюнер подаёт управляющий сигнал и ждёт стабилизации значения с датчика
	2. Тюнер изменяет сигнал на некоторую величину (ступеньку)
	3. Ждёт заданное время, затем меняет сигнал на ту же ступеньку, но в другую сторону
	4. Начинается раскачка системы: при прохождении значения с датчика через значение стабилизации сигнал снова переключается
	5. Производится анализ периода раскачки и её амплитуды, на основании этих данных вычисляются рекомендуемые коэффициенты
	
	=== Как пользоваться библиотекой? ===
	1. Инициализация и настройка
	PIDtuner tuner;
	tuner.setParameters(направление, сигнал, ступенька, период, точность стабилизации, продолж. импульса, период итерации);
	
	1.1 Направление:
		- NORMAL: увеличение выходного сигнала увеличивает сигнал с датчика (например обогреватель, мотор)
		- REVERSE: увеличение выходного сигнала уменьшает сигнал с датчика (например холодильник, тормоз)	
	1.2 Cигнал: базовый сигнал на управляющее устройство. Система будет ждать стабилизации по величине этого сигнала, и от него будет откладываться ступенька	
	1.3 Ступенька: величина, на которую будет изменяться сигнал в обе стороны от базового	
	1.4 Период: период опроса в ожидании стабилизации
	1.5 Точность стабилизации: скорость изменения значения с датчика, ниже которой система будет считаться стабильной	
	1.6 Продолж. импульса: время в миллисекундах на первую раскачку	
	1.7 Период итерации: dt системы в мс, желательно должно совпадать с периодом ПИД регулятора
	
	2. Структура цикла
	Библиотека сделана универсальной для любого датчика и управляющего устройства, цикл тюнинга организуется вот так:
	// цикл
	tuner.setInput(значение с датчика);		// передаём текущее значение с датчика. ЖЕЛАТЕЛЬНО ФИЛЬТРОВАННОЕ
	tuner.compute();						// тут производятся вычисления по своему таймеру
	// tuner.getOutput(); // тут можно забрать новый управляющий сигнал
	analogWrite(pin, tuner.getOutput());	// например для ШИМ
	
	3. Отладка и получение значений
	3.1 Во время работы тюнера можно вызвать tuner.getAccuracy() - чем ближе его значение к 100, тем стабильнее на данный момент качается система и 
	тем вычисляемые коэффициенты будут более близки к идеальным
	3.2 Для наблюдения за тюнером через Serial есть готовые методы:
		- tuner.debugText() выводит текстовые данные (смотри скриншот в папке docs библиотеки)
		- tuner.debugPlot() выводит данные для построения графика через плоттер Arduino IDE (смотри скриншот в папке docs библиотеки)
	3.3 Чтобы получить коэффициенты внутри программы (без Serial) желательно задатьт условие 
	if (tuner.getAccuracy() > 95) и при наступлении этого условия получить коэффициенты:
	
	tuner.getPI_p() - p для ПИ регулятора
	tuner.getPI_i() - i для ПИ регулятора
	
	tuner.getPID_p() - p для ПИД регулятора
	tuner.getPID_i() - i для ПИД регулятора
	tuner.getPID_d() - d для ПИД регулятора	
*/

#define BUF_SIZE 8			// размер буфера линеаризации
#define TUNER_COEF_TYPE 0	// коэффициенты перевода (набор 0 или набор 1)

// ===========================================================================
#include <Arduino.h>
#define NORMAL 0
#define REVERSE 1

#if (TUNER_COEF_TYPE == 0)
// не помню где нашёл
#define TUNE_P_PI 0.4
#define TUNE_I_PI 0.48
#define TUNE_P_PID 0.6
#define TUNE_I_PID 1.2
#define TUNE_D_PID 0.075
#elif (TUNER_COEF_TYPE == 1)
// Ziegler-Nichols
#define TUNE_P_PI 0.45
#define TUNE_I_PI 0.83
#define TUNE_P_PID 0.59
#define TUNE_I_PID 0.5
#define TUNE_D_PID 0.125
#endif

class PIDtuner {
public:
	void setParameters(bool newDirection, int newSteady, int newStep, int newWait, float newWindow, uint16_t newPulse, int newPeriod) {
		steady = newSteady;
		step = newDirection ? -newStep : newStep;
		wait = newWait;
		window = newWindow;
		pulse = newPulse;
		period = newPeriod;
	}

	void setInput(float input) {
		thisValue = input;
	}	
	
	void compute() {		
		if (millis() - tmr >= period) {
			tmr = millis();
			
			// перемотка массива и расчёт производной через линеаризацию МНК
			for (byte i = 0; i < BUF_SIZE - 1; i++) buf[i] = buf[i + 1];
			buf[BUF_SIZE - 1] = thisValue;
			derivative();
			realValue = thisDerivative * (BUF_SIZE - 1) + b;	// аппрокс. значение

			switch (state) {
			case 0:
				state = 1;
				startTime = millis();
				output = (steady);				
				trigger = true;
				changeDir = false;
				oscTime = 1;
				prevOscTime = 0;
				break;

			case 1:   // ждём стабилизации значения
				if (millis() - startTime >= wait) {  // ждём стабилизации			
					startTime = millis();
					if (abs(prevValue - realValue) < window) {
						state = 2;
						steadyValue = realValue;	// приняли значение как установившееся
						maxVal = steadyValue;
						minVal = steadyValue;
						output = (steady - step);	// дёргаем вниз
					}
					prevValue = realValue;
					debFlag = true;
				}
				break;

			case 2:   // ждём и дёргаем обратно, раскачиваем систему
				if (millis() - startTime >= pulse) {
					state = 3;
					startTime = millis();
					output = (steady + step);					
					debFlag = true;
				}
				break;

			case 3:   // анализируем качели
				if (thisDerivative < 0) {      			// значение падает
					if (changeDir && !trigger) {		// верхняя точка
						changeDir = false;
						maxVal = thisDerivative * (BUF_SIZE / 2 - 1) + b;	// взяли средний из массива
					}
					if (realValue < steadyValue) {  	// проскочили среднюю линию
						if (!trigger) {
							trigger = 1;
							output = (steady + step); 	// включили в обратную
						}
					}
				} else {             					// значение растёт
					if (!changeDir && trigger) {     	// нижняя точка
						changeDir = true;
						minVal = thisDerivative * (BUF_SIZE / 2 - 1) + b;	// взяли средний из массива
					}
					if (realValue > steadyValue) {    	// проскочили среднюю линию
						if (trigger) {
							trigger = 0;
							output = (steady - step);   // включили в обратную							

							// анализ времени цикла
							prevOscTime = oscTime;
							oscTime = millis() - startTime;
							accuracy = 100.0f * min(oscTime, prevOscTime) / max(oscTime, prevOscTime);
							startTime = millis();

							float Ku = abs(4.0 * step / (maxVal - minVal) / PI);
							float Pu = oscTime / 1000.0;
							PI_k[0] = TUNE_P_PI * Ku;
							PI_k[1] = TUNE_I_PI * Ku / Pu;
							PID_k[0] = TUNE_P_PID * Ku;
							PID_k[1] = TUNE_I_PID * Ku / Pu;
							PID_k[2] = TUNE_D_PID * Ku * Pu;

							debFlag = true;
						}
					}
				}
				break;
			}
		}
	}
	
	float getPI_p() {return PI_k[0];}
	float getPI_i() {return PI_k[1];}
	float getPID_p() {return PID_k[0];}
	float getPID_i() {return PID_k[1];}
	float getPID_d() {return PID_k[2];}

	int getOutput() {
		return output;
	}

	byte getAccuracy() {
		return (state == 3) ? accuracy : 0;
	}
	
	byte getState() {
		return state;
	}

	void reset() {
		state = 0;
	}

	void debugText(Stream* port = &Serial) {
		if (debFlag) {
			debFlag = false;
			switch (state) {
			case 1: port->print("stabilize: "); port->println(realValue);
				break;
			case 2: port->println("wait pulse");
				break;
			case 3: port->print("analysis: "); port->print(accuracy); port->print("%\t");
				port->print("PI p: "); port->print(PI_k[0]); port->print('\t');
				port->print("PI i: "); port->print(PI_k[1]); port->print('\t');
				port->print("PID p: "); port->print(PID_k[0]); port->print('\t');
				port->print("PID i: "); port->print(PID_k[1]); port->print('\t');
				port->print("PID d: "); port->print(PID_k[2]); port->println();
				break;
			}
		}
	}
	void debugPlot(Stream* port = &Serial) {
		if (millis() - debTmr > period) {
			debTmr = millis();
			port->print(output); port->print(' ');
			port->print(thisValue); port->print(' ');
			port->print(realValue); port->print(' ');
			port->print(minVal); port->print(' ');
			port->println(maxVal);
		}
	}

	void derivative() {
		// линейный МНК
		float sumX = 0, sumY = 0, sumX2 = 0, sumXY = 0;
		for (int i = 0; i < BUF_SIZE; i++) {
			sumX += i;
			sumY += buf[i];
			sumX2 += i * i;
			sumXY += buf[i] * i;
		}
		thisDerivative = (float)(BUF_SIZE * sumXY - sumX * sumY) / (BUF_SIZE * sumX2 - sumX * sumX);
		b = (float)(sumY - thisDerivative * sumX) / BUF_SIZE;
	}

private:
	bool debFlag;
	int accuracy;
	int steady, step, wait;
	float window;
	uint16_t pulse, period;
	float steadyValue, thisValue, prevValue, realValue, thisDerivative, b;
	uint32_t startTime, tmr, debTmr;
	int output;
	byte state = 0;
	bool trigger = true;
	bool changeDir = false;
	float maxVal, minVal;
	float buf[BUF_SIZE];
	int32_t oscTime = 0, prevOscTime = 0;
	float PI_k[2];
	float PID_k[3];
};
#endif