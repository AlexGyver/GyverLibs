#pragma once
/*
	Автоматический калибровщик коэффициентов ПИД регулятора, метод Cohen-Coon https://pages.mtu.edu/~tbco/cm416/cctune.html
	Данный тюнер лучше настраивает коэффициенты для переходного процесса, например разогрев с одной температуры до другой
	Версия 1.0
	
	=== Как это работает? ===
	1. Тюнер подаёт стартовый управляющий сигнал и ждёт стабилизации значения с датчика
	2. Тюнер запоминает минимальное значение и подаёт конечный сигнал, ждёт стабилизации
	3. Тюнер запоминает максимальное значение, снова подаёт начальный сигнал и ждёт стабилизации
	4. Тюнер снова подаёт конечный сигнал
	5. Зная полное время процесса, тюнер измеряет сигнал в определённых точках и по специальным формулам считает коэффициенты
	
	=== Как пользоваться библиотекой? ===
	1. Инициализация и настройка
	PIDtuner tuner;
	tuner.setParameters(направление, начальный сигнал, конечный сигнал, период, точность, время стабилизации, период итерации)
	
	1.1 Направление:
		- NORMAL: увеличение выходного сигнала увеличивает сигнал с датчика (например обогреватель, мотор)
		- REVERSE: увеличение выходного сигнала уменьшает сигнал с датчика (например холодильник, тормоз)	
	1.2 Начальный сигнал: стартовый сигнал на управляющее устройство
	1.3 Конечный сигнал: конечный сигнал на управляющее устройство
	1.4 Период: период опроса в ожидании стабилизации
	1.5 Точность стабилизации: скорость изменения значения с датчика, ниже которой система будет считаться стабильной
	1.6 Период итерации: dt системы в мс, желательно должно совпадать с периодом ПИД регулятора
	
	2. Структура цикла
	Библиотека сделана универсальной для любого датчика и управляющего устройства, цикл тюнинга организуется вот так:
	// цикл
	tuner.setInput(значение с датчика);		// передаём текущее значение с датчика. ЖЕЛАТЕЛЬНО ФИЛЬТРОВАННОЕ
	tuner.compute();						// тут производятся вычисления по своему таймеру
	// tuner.getOutput(); // тут можно забрать новый управляющий сигнал
	analogWrite(pin, tuner.getOutput());	// например для ШИМ
	
	3. Отладка и получение значений
	3.1 Во время работы тюнера можно вызвать tuner.getState() - вернёт номер текущего этапа работы. На 7-ом этапе можно забирать коэффициенты
	3.2 Для наблюдения за тюнером через Serial есть готовые методы:
		- tuner.debugText() выводит текстовые данные (смотри скриншот в папке docs библиотеки)
		- tuner.debugPlot() выводит данные для построения графика через плоттер Arduino IDE (смотри скриншот в папке docs библиотеки)
	3.3 Чтобы получить коэффициенты внутри программы (без Serial) желательно задать условие 
	if (tuner.getState() == 7) и при наступлении этого условия получить коэффициенты:
	
	tuner.getPI_p() - p для ПИ регулятора
	tuner.getPI_i() - i для ПИ регулятора
	
	tuner.getPID_p() - p для ПИД регулятора
	tuner.getPID_i() - i для ПИД регулятора
	tuner.getPID_d() - d для ПИД регулятора	
*/

// ===========================================================================
#include <Arduino.h>
#define NORMAL 0
#define REVERSE 1

class PIDtuner2 {
public:
	void setParameters(bool newDirection, int newStart, int newEnd, int newWait, float newWindow, int newPeriod) {
		direction = newDirection;
		start = newStart;
		end = newEnd;
		wait = newWait;
		window = newWindow;
		period = newPeriod;
	}

	void setInput(float input) {
		thisValue = input;
	}	
	
	void compute() {		
		if (millis() - tmr >= period) {
			tmr = millis();
			switch (state) {
			case 0:		// старт
				output = (start);
				startTime = millis();
				state = 1;
				debFlag = true;
				break;
			case 1:		// ловим плато				
				if (millis() - startTime > wait) {
					startTime = millis();
					if (abs(thisValue - lastValue) < window) {
						state = 2;						
						startValue = thisValue;
						output = (end);						
					}					
					lastValue = thisValue;
					debFlag = true;
				}
				break;
			case 2:		// ловим второе плато				
				if (millis() - startTime > wait) {
					startTime = millis();
					if (abs(thisValue - lastValue) < window) {
						state = 3;						
						endValue = thisValue;
						B = abs(endValue - startValue);
						output = (start);
					}
					lastValue = thisValue;
					debFlag = true;
				}
				break;
			case 3:		// ждём остывания до 1				
				if (millis() - startTime > wait) {
					startTime = millis();
					if (abs(thisValue - lastValue) < window) {
						state = 4;						
						output = (end);
					}
					lastValue = thisValue;
					debFlag = true;
				}
				break;        
			case 4:		// ловим t2				
				if ( (!direction && thisValue > (startValue + B / 2.0f)) ||
						(direction && thisValue < (startValue + B / 2.0f)) ) {
					t2 = (millis() - startTime) / 1000.0f;
					state = 5;
					debFlag = true;
				}
				break;
			case 5:		// ловим t3				
				if ( (!direction && thisValue > (startValue + B * 0.632f)) ||
						(direction && thisValue < (startValue + B * 0.632f)) ) {
					state = 6;
					debFlag = true;
					t3 = (millis() - startTime) / 1000.0f;
				}
				break;
			case 6:		// считаем				
				float Kc, Ki, Kd, tauI, tauD;
				float t1 = (t2 - 0.693f * t3) / 0.307f;
				float K = B / abs(end - start);
				float r = t1 / (t3 - t1);

				// PI рег
				Kc = (0.9 + r / 12.0) / (r * K);
				tauI = t1 * (30.0 + 3.0 * r) / (9.0 + 20.0 * r);
				Ki = Kc / tauI;
				PI_k[0] = Kc;
				PI_k[1] = Ki;

				// PID рег
				Kc = (1.33 + r / 4.0) / (r * K);
				tauI = t1 * (32.0 + 6.0 * r) / (13.0 + 8.0 * r);
				tauD = t1 * 4.0 / (11.0 + 2.0 * r);
				Ki = Kc / tauI;
				Kd = Kc * tauD;
				PID_k[0] = Kc;
				PID_k[1] = Ki;
				PID_k[2] = Kd;
				state = 7;		// конец
				debFlag = true;
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

	byte getState() {
		return state;
	}

	void reset() {
		state = 0;
	}

	void debugText() {
		if (debFlag) {
			debFlag = false;
			switch (state) {
			case 1: Serial.print("stabilize down: "); Serial.println(thisValue);
				break;
			case 2: Serial.print("stabilize up: "); Serial.println(thisValue);
				break;
			case 3: Serial.print("stabilize down: "); Serial.println(thisValue);
				break;
			case 4: Serial.println("got t2");
				break;
			case 5: Serial.println("got t3");
				break;
			case 6: Serial.println("compute");
				break;			
			case 7: Serial.print("result: ");
				Serial.print("PI p: "); Serial.print(PI_k[0]); Serial.print('\t');
				Serial.print("PI i: "); Serial.print(PI_k[1]); Serial.print('\t');
				Serial.print("PID p: "); Serial.print(PID_k[0]); Serial.print('\t');
				Serial.print("PID i: "); Serial.print(PID_k[1]); Serial.print('\t');
				Serial.print("PID d: "); Serial.print(PID_k[2]); Serial.println();
				break;
			}
		}
	}
	void debugPlot() {
		if (millis() - debTmr > period) {
			debTmr = millis();
			Serial.println(thisValue);			
		}
	}

private:
	bool debFlag, direction;
	byte state = 0;
	int start, end, wait, period, output;
	float window;
	uint32_t startTime, tmr, debTmr;
	float thisValue, lastValue = 0.0;
	float endValue = 0.0;
	float startValue = 0.0;
	float B = 0.0;
	float t2, t3;
	float PI_k[2];
	float PID_k[3];
};