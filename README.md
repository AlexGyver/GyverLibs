# GyverLibs by AlexGyver
## Написанные с нуля/модифицированные библиотеки для Arduino
[![AlexGyver YouTube](http://alexgyver.ru/git_banner.jpg)](https://www.youtube.com/channel/UCgtAOyEQdAyjvm9ATCi_Aig?sub_confirmation=1)
> Внимание! Библиотеки написаны ютубером-ардуинщиком, возможна высокая концентрация костылей и стандартных подходов. Но всё работает замечательно!
## Содержание
### Железки
* [GyverButton](#GyverButton) - расширенный контроль кнопкой
* [GyverEncoder](#GyverEncoder) - расширенный контроль с энкодером
* [GyverMotor](#GyverMotor) - управление драйверами моторов
* [AccelMotor](#AccelMotor) - управление мотором с обратной связью
* [GyverRGB](#GyverRGB) - управление RGB светодиодами и лентами
* [GyverTM1637](#GyverTM1637) - управление дисплеем на TM1637
* [ServoSmooth](#ServoSmooth) - плавное управление сервоприводами

### Периферия МК
* [directADC](#directADC) - расширенный контроль АЦП и компаратора
* [directTimers](#directTimers)	- расширенный контроль за таймерами-счётчиками
* [GyverPWM](#GyverPWM) - расширенная генерация ШИМ сигнала
* [GyverWDT](#GyverWDT) - расширенный контроль за watchdog

### Алгоритмы
* [PWMrelay](#PWMrelay) - сверхнизкочастотный ШИМ для релюшек
* [GyverFilters](#GyverFilters) - набор фильтров значений
* [GyverTimer](#GyverTimer) - простой таймер для организации кода
* [GyverPID](#GyverPID) - библиотека ПИД регулятора
* [GyverRelay](#GyverRelay) - библиотека релейного регулятора
* [GyverRTOS](#GyverRTOS) - система "реального времени" со сном и менеджером задач

### Системное
* [buildTime](#buildTime) - получение времени компиляции в численном виде
* ~~[GyverTimer012](#GyverTimer012) - настройка прерываний по таймерам 0/1/2~~ Вместо неё теперь есть **GyverTimers**
* [GyverTimers](#GyverTimers) - настройка прерываний по аппаратным таймерам ATmega328 и ATmega2560
* [GyverHacks](#GyverHacks) - набор различных хаков с микроконтроллером
* [GyverPower](#GyverPower) - библиотека для управления энергопотреблением МК

### Лёгкие библиотеки
* [GyverBME280](#GyverBME280) - лёгкая библиотека для датчика BME280
* [microLED](#microLED) - ультра-лёгкая библиотека для адресных диодов
* [microWire](#microWire) - микро библиотека для работы с i2c
* [microDS3231](#microDS3231) - лёгкая библиотека для RTC DS3231
* [microLiquidCrystal_I2C](#microLiquidCrystal_I2C) - облегчённая библиотека для LCD дисплея
* [microDS18B20](#microDS18B20) - микро библиотека для работы с датчиком температуры ds18b20
* [GyverUART](#GyverUART) - облегчённый и ускоренный Serial
* [minimLibs](#minimLibs) - набор классов для работы с железками

---

<a id="PWMrelay"></a>
### PWMrelay v1.0 [СКАЧАТЬ](https://github.com/AlexGyver/GyverLibs/releases/download/PWMrelay/PWMrelay.zip), [ДОКУМЕНТАЦИЯ](https://alexgyver.ru/pwmrelay/)
Библиотека для генерации сверхнизкочастотного ШИМа для релюшек
- Встроенный таймер на миллис
- Установка частоты, скважности и уровня реле

#### Методы и функции библиотеки
<details>
<summary>РАЗВЕРНУТЬ</summary>
<p>
Смотри примеры в папке examples!

```C
PWMrelay(byte pin, bool dir, int period);   // пин, уровень реле HIGH/LOW, период
void tick();                                // тик, вызывать как можно чаще, сам управляет реле
void setPWM(byte duty);                     // установить величину ШИМ, 0-255. При значении 0 и 255 тик неактивен!
byte getPWM();                              // возвращает величину ШИМ
void setPeriod(int period);                 // установить период ШИМ в миллисек. (по умолч. 1000мс == 1с)
int getPeriod();                            // получить период
void setLevel(bool level);                  // установить установить уровень реле (HIGH/LOW)
```
</p>
</details>

---

<a id="microLED"></a>
### microLED v2.3 [СКАЧАТЬ](https://github.com/AlexGyver/GyverLibs/releases/download/microLED/microLED.zip), [ДОКУМЕНТАЦИЯ](https://alexgyver.ru/microled/)
microLED - ультра-лёгкая библиотека для работы с адресной лентой/матрицей
- Основная фишка: сжатие цвета, код занимает в разы меньше места в SRAM по сравнению с аналогами (FastLED, NeoPixel и др.)
	- Использование 8 битного цвета занимает в 3 раза меньше SRAM чем у других библиотек
	- Использование 16 битного цвета занимает в 2/3 раза меньше SRAM чем у других библиотек
- Поддержка сжатия цвета: 8, 16 и 24 бита
- Поддержка порядка цветов: RGB, GRB, BRG
- Работа с цветом:
	- RGB
	- HSV
	- HEX цвета
	- "Цветовое колесо" (1500 самых ярких оттенков)
	- 17 встроенных цветов
- Возможность чтения сжатого цвета в HEX 0xRRGGBB
- Ограничение тока (автокоррекция макс. яркости)
- Быстрейший протокол данных
- Функция уменьшения яркости пикселя
- Поддержка работы с адресными матрицами (см. пример)	
- Поддержка чипов: 2811/2812, остальные не проверялись
- Частичная совместимость со скетчами для FastLED (смотри пример fastToMicro)

Частота обновления (**Гц**) от количества светодиодов (сравнение с другими библиотеками)

Кол-во диодов | FastLED 24-bit | NeoPixel 24-bit | WS2812 24-bit | microLED 24-bit | microLED 16-bit | microLED 8-bit |
--------------|----------------|-----------------|---------------|-----------------|-----------------|----------------|
8			  |400	           |1818	         |3343	         |3436	           |3173	         |2800	          |
16	          |400	           |1264	         |1760	         |1728	           |1594	         |1409	          |
50	          |333	           |554	             |589	         |556	           |513	             |453	          |
100	          |220	           |301	             |298	         |279	           |257	             |227	          |
500	          |60	           |65	             |60	         |56	           |51	             |45	          |

Занимаемая память (**байт**) от количества диодов, где **LED** - количество светодиодов (сравнение с другими библиотеками)

Память        | FastLED 24-bit | NeoPixel 24-bit | WS2812 24-bit | microLED 24-bit | microLED 16-bit | microLED 8-bit |
--------------|----------------|-----------------|---------------|-----------------|-----------------|----------------|
Flash		  |2786	           |1984	         |946	         |306	           |346	             |324	          |
SRAM	      |90+3*LED        |40+3*LED         |31+3*LED       |20+3*LED         |20+2*LED         |20+1*LED        |

#### Методы и функции библиотеки
<details>
<summary>РАЗВЕРНУТЬ</summary>
<p>
Смотри примеры в папке examples!

```C
// ============ Методы класса microLED ============
// ЛЕНТА: имя буфера, количество ледов, пин
microLED(LEDdata *LEDarray, int LEDamount, byte pin);

// МАТРИЦА: имя буфера, пин, ширина матрицы, высота матрицы, тип матрицы, угол подключения, направление	(см. ПОДКЛЮЧЕНИЕ МАТРИЦЫ)
microLED(LEDdata *LEDarray, byte pin, byte width, byte height, M_type type, M_connection conn, M_dir dir);

// лента и матрица
void setRGB(int num, byte r, byte g, byte b);   // RGB
void setHSV(int num, byte h, byte s, byte v);   // HSV
void setHEX(int num, uint32_t color);           // HEX
void setColor(int num, COLORS color);           // стандартный цвет (см. "Стандартные цвета")
void colorWheel(int num, int color);            // цвет 0-1530	
void fill(LEDdata color);                       // заливка цветом (mRGB, mWHEEL, mHEX, mHSV)
void setLED(int n, LEDdata color);              // ставим цвет светодиода (mRGB, mWHEEL, mHEX, mHSV)	
uint32_t getColorHEX(int num);                  // получить HEX цвет диода (для сравнения и т.п.)
LEDdata getColor(int num);                      // получить цвет диода в LEDdata
void fade(int num, byte val);                   // уменьшить яркость на val

void setBrightness(byte newBright);             // яркость 0-255
void clear();                                   // очистка
void show();                                    // отправка

// матрица
void setPix(int x, int y, LEDdata color);       // ставим цвет пикселя x y в LEDdata (mRGB, mWHEEL, mHEX, mHSV)
uint32_t getColorHEX(int x, int y);             // получить цвет пикселя в HEX
LEDdata getColor(int x, int y);                 // получить цвет пикселя в LEDdata
void fadePix(int x, int y, byte val);           // уменьшить яркость пикселя на val
uint16_t getPixNumber(int x, int y);            // получить номер пикселя в ленте по координатам

// ============ Функции кодирования цвета ============
LEDdata mRGB(byte r, byte g, byte b);   // RGB 255, 255, 255
LEDdata mWHEEL(int color);              // цвета 0-1530
LEDdata mHEX(uint32_t color);           // HEX цвет
LEDdata mHSV(byte h, byte s, byte v);   // HSV 255, 255, 255
LEDdata mCOLOR(COLORS color);           // цвет

// ==================== Константы ====================
// Стандартные цвета
WHITE
SILVER
GRAY
BLACK
RED
MAROON
YELLOW
OLIVE
LIME
GREEN
AQUA
TEAL
BLUE
NAVY
PINK
PURPLE
```
</p>
</details>
---

<a id="GyverPower"></a>
### GyverPower v1.4 [СКАЧАТЬ](https://github.com/AlexGyver/GyverLibs/releases/download/GyverPower/GyverPower.zip), [ДОКУМЕНТАЦИЯ](https://alexgyver.ru/gyverpower/)
GyverPower - библиотека для расширенного управления энергопотреблением микроконтроллера
- Управление системным клоком
- Включение/выключение периферии:
	- BOD
	- Таймеры
	- I2C/UART/SPI
	- USB
	- ADC
- Сон в разных режимах
- Сон на любой период
	- Калибровка таймера для точных периодов сна
	- Коррекция миллис на время сна
- Поддерживаемые МК
	- Atmega2560/32u4/328
	- Attiny85/84/167
- Разработано by Egor 'Nich1con' Zaharov
- Версия 1.1 несовместима с 1.0! См. примеры (добавлен класс)
#### Методы и функции библиотеки
<details>
<summary>РАЗВЕРНУТЬ</summary>
<p>
Смотри примеры в папке examples!

```C
void hardwareEnable(uint16_t data);				// включение указанной периферии (см. ниже "Константы периферии")
void hardwareDisable(uint16_t data);			// выключение указанной периферии (см. ниже "Константы периферии")
void setSystemPrescaler(prescalers_t prescaler);// установка делителя системной частоты (см. ниже "Константы делителя")
void bodInSleep(bool en);						// Brown-out detector в режиме сна (true вкл - false выкл) по умолч. отключен!
void setSleepMode(sleepmodes_t mode);			// установка текущего режима сна (см. ниже "Режимы сна")
void autoCalibrate(void);						// автоматическая калибровка таймера сна, выполняется 2 секунды
uint16_t getMaxTimeout(void);					// возвращает реальный период "8 секунд", выполняется ~8 секунд
void calibrate(uint16_t ms);					// ручная калибровка тайм-аутов watchdog для sleepDelay (ввести макс период из getMaxTimeout)
void sleep(sleepprds_t period);					// сон на фиксированный период (см. ниже "Периоды сна")
uint8_t sleepDelay(uint32_t ms);				// сон на произвольный период в миллисекундах (до 52 суток), возвращает остаток времени для коррекции таймеров
void correctMillis(bool state);					// корректировать миллис на время сна sleepDelay() (по умолчанию включено)
void wakeUp(void);								// помогает выйти из sleepDelay прерыванием (вызывать в будящем прерывании)	

/* 
	РЕЖИМЫ СНА для setSleepMode()
	
	IDLE_SLEEP			- Легкий сон, отключается только клок CPU и Flash, просыпается от любых прерываний
	ADC_SLEEP			- Легкий сон, отключается CPU и system clock, АЦП начинает преобразование при уходе в сон (см. пример ADCinSleep)	
	POWERDOWN_SLEEP		- Наиболее глубокий сон, отключается всё кроме WDT и внешних прерываний, просыпается от аппаратных (обычных + PCINT) или WDT за 1000 тактов (62 мкс)
	STANDBY_SLEEP		- Глубокий сон, идентичен POWERDOWN_SLEEP + system clock активен, пробуждение за 6 тактов (0.4 мкс)
	POWERSAVE_SLEEP		- Глубокий сон, идентичен POWERDOWN_SLEEP + timer 2 активен (+ можно проснуться от его прерываний), можно использовать для счета времени (см. пример powersaveMillis)
	EXTSTANDBY_SLEEP	- Глубокий сон, идентичен POWERSAVE_SLEEP + system clock активен, пробуждение за 6 тактов (0.4 мкс)
*/

/* 
	ПЕРИОДЫ СНА для sleep()
	
	SLEEP_16MS
	SLEEP_32MS
	SLEEP_64MS
	SLEEP_128MS
	SLEEP_256MS
	SLEEP_512MS
	SLEEP_1024MS
	SLEEP_2048MS
	SLEEP_4096MS
	SLEEP_8192MS
	SLEEP_FOREVER	- вечный сон без таймера
*/

/* 
	КОНСТАНТЫ ДЕЛИТЕЛЯ для setSystemPrescaler()
	
	PRESCALER_1
	PRESCALER_2
	PRESCALER_4
	PRESCALER_8
	PRESCALER_16
	PRESCALER_32
	PRESCALER_64
	PRESCALER_128
	PRESCALER_256
*/

/* 
	КОНСТАНТЫ ПЕРИФЕРИИ для hardwareDisable() и hardwareEnable()
	
	PWR_ALL		- всё железо
	PWR_ADC		- АЦП и компаратор
	PWR_TIMER1	- Таймер 0
	PWR_TIMER0	- Таймер 1
	PWR_TIMER2	- Таймер 2
	PWR_TIMER3	- Таймер 3
	PWR_TIMER4	- Таймер 4
	PWR_TIMER5	- Таймер 5	
	PWR_UART0	- Serial 0
	PWR_UART1	- Serial 1
	PWR_UART2	- Serial 2
	PWR_UART3	- Serial 3
	PWR_I2C		- Wire
	PWR_SPI		- SPI
	PWR_USB		- USB	
	PWR_USI		- Wire + Spi (ATtinyXX)
	PWR_LIN		- USART LIN (ATtinyXX)
*/
```
</p>
</details>

---

<a id="minimLibs"></a>
### minimLibs v1.2 [СКАЧАТЬ](https://github.com/AlexGyver/GyverLibs/releases/download/minimLibs/minimLibs.zip)
minimLibs это набор классов, являющихся облегчёнными и упрощёнными версиями библиотек, на данный момент таких как:
- GyverButton (работа с кнопкой)
- GyverEncoder (работа с энкодером)
- GyverTimer (таймер на millis() )
- GyverTimer v2 (улучшенный таймер на millis() )
- Термистор
- DS18B20 (датчик температуры)

Созданы для работы со скетчами, требовательными к памяти. **НЕ НУЖНО ПОМЕЩАТЬ ИХ В ПАПКУ С БИБЛИОТЕКАМИ!** Примеры из minimLibs живут отдельной жизнью в сврих папках, им не нужны библиотеки. Смотрите примеры!

---

<a id="microBME280"></a>
### microBME280 v1.4 [СКАЧАТЬ](https://github.com/AlexGyver/GyverLibs/releases/download/microWire/microBME280.zip), [ДОКУМЕНТАЦИЯ](https://alexgyver.ru/microlibs/)
Лёгкая библиотека для работы с датчиком BME280
- Легче аналогов =)
- Разработано by Egor 'Nich1con' Zaharov
#### Методы и функции библиотеки
<details>
<summary>РАЗВЕРНУТЬ</summary>
<p>
Смотри примеры в папке examples!

```C
GyverBME280();								// Create an object of class BME280
bool begin(void);							// Initialize sensor with standard or previously selected parameters
bool isMeasuring(void);						// Returns 'true' while the measurement is in progress					
float readPressure(void);					// Read and calculate atmospheric pressure [float , Pa]
float readHumidity(void);					// Read and calculate air humidity [float , %]
void oneMeasurement(void);					// Make one measurement and go back to sleep [FORCED_MODE only]
void setMode(uint8_t mode);
float readTemperature(void);				// Read and calculate air temperature [float , *C]
void setFilter(uint8_t mode);				// Adjust the filter ratio other than the standard one [before begin()]
void setStandbyTime(uint8_t mode);			// Adjust the sleep time between measurements [NORMAL_MODE only][before begin()]
void setHumOversampling(uint8_t mode);		// Set oversampling or disable humidity module [before begin()]
void setTempOversampling(uint8_t mode);		// Set oversampling or disable temperature module [before begin()]
void setPressOversampling(uint8_t mode);	// Set oversampling or disable pressure module [before begin()]
```
</p>
</details>

---

<a id="microWire"></a>
### microWire v2.1 [СКАЧАТЬ](https://github.com/AlexGyver/GyverLibs/releases/download/microWire/microWire.zip), [ДОКУМЕНТАЦИЯ](https://alexgyver.ru/microlibs/)
Лёгкая библиотека с минимальным набором инстурментов для работы с модулями по шине I2C
- Не является полноценным аналогом библиотеки Wire!!!
- Не все библиотеки на Wire смогут работать с microWire
- microWire создана для работы с устройствами в ручном режиме
- Разработано by Egor 'Nich1con' Zaharov
#### Методы и функции библиотеки
<details>
<summary>РАЗВЕРНУТЬ</summary>
<p>
Смотри примеры в папке examples!

```C
void begin(int baudrate = 400);
void setClock(uint32_t clock);
void beginTransmission(void);
void beginTransmission(uint8_t addr);
void endTransmission(void);
void write(uint8_t data);
void requestFrom(uint8_t addr);
uint8_t receive_ack(void);
uint8_t receive_nack(void);
```
</p>
</details>

---

<a id="microDS3231"></a>
### microDS3231 v1.4 [СКАЧАТЬ](https://github.com/AlexGyver/GyverLibs/releases/download/microDS3231/microDS3231.zip), [ДОКУМЕНТАЦИЯ](https://alexgyver.ru/microlibs/)
Ультра лёгкая библиотека для работы с RTC DS3231
- Для работы нужна библиотека **microWire.h**
- Разработано by Egor 'Nich1con' Zaharov
#### Методы и функции библиотеки
<details>
<summary>РАЗВЕРНУТЬ</summary>
<p>
Смотри примеры в папке examples!

```C
MicroDS3231();                  // конструктор
void setTime(uint8_t seconds , uint8_t minutes , uint8_t hours , uint8_t date, uint8_t month, uint16_t year);	// установка времени
void setTime(uint8_t param);    // установка времени == времени компиляции
bool lostPower(void);           // проверка на сброс питания
uint8_t getSeconds(void);       // получить секунды
uint8_t getMinutes(void);       // получить минуты
uint8_t getHours(void);         // получить часы
uint8_t getDay(void);           // получить день недели
uint8_t getDate(void);          // получить число
uint16_t getYear(void);         // получить год
uint8_t getMonth(void);         // получить месяц
```
</p>
</details>

---

<a id="microDS18B20"></a>
### microDS18B20 v2.2 [СКАЧАТЬ](https://github.com/AlexGyver/GyverLibs/releases/download/microDS18B20/microDS18B20.zip), [ДОКУМЕНТАЦИЯ](https://alexgyver.ru/microlibs/)
Ультра лёгкая библиотека для работы с датчиком температуры Dallas DS18B20
- Разработано by Egor 'Nich1con' Zaharov
#### Методы и функции библиотеки
<details>
<summary>РАЗВЕРНУТЬ</summary>
<p>
Смотри примеры в папке examples!

```C
MicroDS18B20(uint8_t pin);  // конструктор
void requestTemp();         // запросить температуру
float getTemp(void);        // получить температуру
```
</p>
</details>

---

<a id="microLiquidCrystal_I2C"></a>
### microLiquidCrystal_I2C v1.1 [СКАЧАТЬ](https://github.com/AlexGyver/GyverLibs/releases/download/microLiquidCrystal_I2C/microLiquidCrystal_I2C.zip), [ДОКУМЕНТАЦИЯ](https://alexgyver.ru/microlibs/)
Библиотека для LCD дисплея с переходником на I2C, облегчена за счёт использования microWire
- Для работы нужна библиотека **microWire.h**

#### Методы и функции библиотеки
<details>
<summary>РАЗВЕРНУТЬ</summary>
<p>
Смотри примеры в папке examples!

```C
	Библиотека полностью совместима с оригинальной LiquidCrystal_I2C
```
</p>
</details>

---

<a id="GyverButton"></a>
### GyverButton v3.4 [СКАЧАТЬ](https://github.com/AlexGyver/GyverLibs/releases/download/GyverButton/GyverButton.zip), [ДОКУМЕНТАЦИЯ](https://alexgyver.ru/gyverbutton/)
Библиотека для многофункциональной отработки нажатия кнопок с Arduino. Возможности:
- Работа с нормально замкнутыми и нормально разомкнутыми кнопками
- Работа с подключением PULL_UP и PULL_DOWN
- Опрос кнопки с программным антидребезгом контактов
- Настраиваемое время антидребезга
- Отработка нажатия, удерживания, отпускания, клика по кнопке
- Настраиваемый таймаут удержания
- Отработка одиночного, двойного и тройного нажатия (вынесено отдельно)
- Отработка любого количества нажатий кнопки (функция возвращает количество нажатий)
- Настраиваемый таймаут повторного нажатия/удержания
- Функция изменения значения переменной с заданным шагом и заданным интервалом по времени
- Возможность опрашивать не кнопку, а напрямую давать величину (все возможности библиотеки для матричных и резистивных клавиатур)
#### Методы и функции библиотеки
<details>
<summary>РАЗВЕРНУТЬ</summary>
<p>
Смотри примеры в папке examples!

```C
GButton(uint8_t pin);
// класс кнопки, принимает пин
	
GButton(uint8_t PIN, boolean type, boolean dir);    
// класс кнопки, принимает PIN пин, тип type (HIGH_PULL / LOW_PULL) и направление dir (NORM_OPEN / NORM_CLOSE)
// HIGH_PULL - кнопка подключена к GND, пин подтянут к VCC, pinMode - INPUT_PULLUP (по умолчанию)
// LOW_PULL - кнопка подключена к VCC, пин подтянут к GND, pinMode - INPUT
// NORM_OPEN - кнопка по умолчанию разомкнута
// NORM_CLOSE - кнопка по умолчанию замкнута
                                                    
void setDebounce(uint16_t debounce);           // установка времени антидребезга (по умолчанию 80 мс)
void setTimeout(uint16_t timeout);             // установка таймаута удержания (по умолчанию 500 мс)	
void setClickTimeout(uint16_t timeout);        // установка таймаута между нажатиями (по умолчанию 300 мс)
void setStepTimeout(uint16_t step_timeout);    // установка таймаута между инкрементами (по умолчанию 400 мс)
void setType(boolean type);                    // установка типа кнопки (HIGH_PULL - подтянута к питанию, LOW_PULL - к gnd)	
void setDirection(boolean dir);                // установка направления (разомкнута/замкнута по умолчанию - NORM_OPEN, NORM_CLOSE)	
	
void setTickMode(boolean tickMode);            // (MANUAL / AUTO) ручной или автоматический опрос кнопки функцией tick()	
                                               // MANUAL - нужно вызывать функцию tick() вручную														
                                               // AUTO - tick() входит во все остальные функции и опрашивается сама
	
void tick();                                   // опрос кнопки	
void tick(boolean state);                      // опрос внешнего значения (0 нажато, 1 не нажато) (для матричных, резистивных клавиатур и джойстиков)
	
boolean isPress();      // возвращает true при нажатии на кнопку. Сбрасывается после вызова
boolean isRelease();    // возвращает true при отпускании кнопки. Сбрасывается после вызова
boolean isClick();      // возвращает true при клике. Сбрасывается после вызова
boolean isHolded();     // возвращает true при удержании дольше timeout. Сбрасывается после вызова
boolean isHold();       // возвращает true при нажатой кнопке, не сбрасывается
boolean state();        // возвращает состояние кнопки

boolean isSingle();     // возвращает true при одиночном клике. Сбрасывается после вызова
boolean isDouble();     // возвращает true при двойном клике. Сбрасывается после вызова
boolean isTriple();     // возвращает true при тройном клике. Сбрасывается после вызова
	
boolean hasClicks();    // проверка на наличие кликов. Сбрасывается после вызова
uint8_t getClicks();    // вернуть количество кликов
uint8_t getHoldClicks();// вернуть количество кликов, предшествующее удерживанию
	
boolean isStep();       // возвращает true по таймеру setStepTimeout, смотри пример
void resetStates();     // сбрасывает все is-флаги и счётчики
```
</p>
</details>

---

<a id="GyverEncoder "></a>
### GyverEncoder v4.7 [СКАЧАТЬ](https://github.com/AlexGyver/GyverLibs/releases/download/GyverEncoder/GyverEncoder.zip), [ДОКУМЕНТАЦИЯ](https://alexgyver.ru/encoder/)
Библиотека для отработки энкодера с Arduino. Возможности:
- Отработка поворота с антидребезгом
- Отработка нажатия кнопки с антидребезгом
- Отработка нажатия и удержания кнопки
- Отработка "нажатого поворота"
- Отработка "быстрого поворота"
- Три алгоритма опроса энкодера
- Работа с виртуальным энкодером
- Работа с двумя типами экнодеров
- Версия 3+ более оптимальная
#### Методы и функции библиотеки
<details>
<summary>РАЗВЕРНУТЬ</summary>
<p>
Смотри примеры в папке examples!

```C
Encoder(uint8_t, uint8_t, uint8_t);
// CLK, DT, SW

Encoder(uint8_t, uint8_t, uint8_t, boolean);
// CLK, DT, SW, тип (TYPE1 / TYPE2): TYPE1 одношаговый, TYPE2 двухшаговый. Если ваш энкодер работает странно, смените тип
		
void tick();                             // опрос энкодера, нужно вызывать постоянно или в прерывании
void setType(boolean type);              // TYPE1 / TYPE2 - тип энкодера TYPE1 одношаговый, TYPE2 двухшаговый. Если ваш энкодер работает странно, смените тип
void setTickMode(boolean tickMode);      // MANUAL / AUTO - ручной или автоматический опрос энкодера функцией tick(). (по умолчанию ручной)
void setDirection(boolean direction);    // NORM / REVERSE - направление вращения энкодера
void setFastTimeout(int timeout);        // установка таймаута быстрого поворота
	
boolean isTurn();                        // возвращает true при любом повороте, сама сбрасывается в false
boolean isRight();                       // возвращает true при повороте направо, сама сбрасывается в false
boolean isLeft();                        // возвращает true при повороте налево, сама сбрасывается в false
boolean isRightH();                      // возвращает true при удержании кнопки и повороте направо, сама сбрасывается в false
boolean isLeftH();                       // возвращает true при удержании кнопки и повороте налево, сама сбрасывается в false
boolean isFastR();                       // возвращает true при быстром повороте
boolean isFastL();                       // возвращает true при быстром повороте
	
boolean isPress();                       // возвращает true при нажатии кнопки, сама сбрасывается в false
boolean isRelease();                     // возвращает true при отпускании кнопки, сама сбрасывается в false
boolean isClick();                       // возвращает true при нажатии и отпускании кнопки, сама сбрасывается в false
boolean isHolded();                      // возвращает true при удержании кнопки, сама сбрасывается в false
boolean isHold();                        // возвращает true при удержании кнопки, НЕ СБРАСЫВАЕТСЯ
```
</p>
</details>

---

<a id="GyverUART"></a>
### GyverUART v1.10 [СКАЧАТЬ](https://github.com/AlexGyver/GyverLibs/releases/download/GyverUART/GyverUART.zip), [ДОКУМЕНТАЦИЯ](https://alexgyver.ru/gyveruart/)
Лёгкая библиотека для работы с последовательным портом. Практически полный аналог стандартного Serial, но гораздо легче и быстрее (например, чтение быстрее в 2 раза). Данная библиотека вшита в моё ядро [GyverCore](https://github.com/AlexGyver/GyverCore), при его использовании дополнительно библиотеку подключать не нужно!

## Сравнение памяти
Функция         | Arduino | GyverUART | Разница, байт 
----------------|---------|-----------|---------------
Serial.begin    | 994     | 236       | 758    
write           | 898     | 254       | 644         
print long      | 1136    | 436       | 700            
print string    | 2260    | 1764      | 496            
print float     | 1168    | 516       | 652           
parseInt        | 1076    | 336       | 740            
readString      | 2378    | 1716      | 662            
parseFloat      | 1108    | 366       | 742      

#### Методы и функции библиотеки
<details>
<summary>РАЗВЕРНУТЬ</summary>

Смотри примеры в папке examples!  
- **uart.begin()** - запустить соединение по последовательному порту со скоростью 9600
- **uart.begin(baudrate)** - запустить соединение по последовательному порту со скоростью baudrate
- **uart.end()** - выключить сериал
- **uart.peek()** - вернуть крайний байт из буфера, не убирая его оттуда
- **uart.clear()** - очистить буфер
- **uart.read()** - вернуть крайний байт из буфера, убрав его оттуда
- **uart.write(val)** - запись в порт
- **uart.print(val)** - печать в порт (числа, строки, char array)
- **uart.println(val)** - печать в порт с переводом строки
- **uart.available()** - возвразает true, если в буфере что-то есть
- **uart.setTimeout(val)** - установить таймаут для функций парсинга (по умолчанию 100 мс)
- **uart.parseInt()** - принять целочисленное число
- **uart.readString()** - принять строку
- **uart.readStringUntil()** - принять строку по терминатору
- **uart.parseFloat()** - принять число float
- **uart.parsePacket(dataArray)** - принять пакет вида **$50 60 70;** в массив dataArray (смотри пример)

</details>

---

<a id="GyverTimers"></a>
### GyverTimers v1.3 [СКАЧАТЬ](https://github.com/AlexGyver/GyverLibs/releases/download/GyverTimers/GyverTimers.zip), [ДОКУМЕНТАЦИЯ](https://alexgyver.ru/GyverTimers/)
Настройка и контроль прерываний по аппаратным таймерам:
- Поддерживаются все три таймера на ATmega328 и шесть таймеров на ATmega2560;		
- Настройка периода (мкс) и частоты (Гц) прерываний:
	- 8 бит таймеры: 31 Гц - 1 МГц (32 258 мкс - 1 мкс);
	- 16 бит таймеры: 0.11 Гц - 1 МГц (9 000 000 мкс - 1 мкс);
- Автоматическая корректировка настройки периода от частоты тактирования (F_CPU);
- Функция возвращает точный установившийся период/частоту для отладки (частота ограничена разрешением таймера);
- Поддержка многоканального режима работы: один таймер вызывает 2 (ATmega328) или 3 (ATmega2560, таймеры 1, 3, 4, 5) прерывания с настраиваемым сдвигом по фазе 0-360 градусов;
- Настраиваемое действие аппаратного вывода таймера по прерыванию: высокий сигнал, низкий сигнал, переключение. Позволяет генерировать меандр (одно- и двухтактный);
- Контроль работы таймера: старт/стоп/пауза/продолжить/инициализация;
#### Методы и функции библиотеки
<details>
<summary>РАЗВЕРНУТЬ</summary>
<p>
Смотри примеры в папке examples!

```C
// N - номер таймера (0, 1... 5)
TimerN.setPeriod(период) - установка периода в микросекундах и запуск таймера. Возвращает реальный период (точность ограничена разрешением таймера).
TimerN.setFrequency(частота) - установка частоты в Герцах и запуск таймера. Возвращает реальную частоту (точность ограничена разрешением таймера).
TimerN.setFrequencyFloat(частота float) - установка частоты в Герцах и запуск таймера, разрешены десятичные дроби. Возвращает реальную частоту (точность ограничена разрешением таймера).
TimerN.enableISR(источник, фаза) - включить прерывания, канал прерываний CHANNEL_A или CHANNEL_B (+ CHANNEL_C у Mega2560), сдвиг фазы 0-359 (без указания параметров будет включен канал А и сдвиг фазы 0).
TimerN.disableISR(источник) - выключить прерывания, канал CHANNEL_A или CHANNEL_B. Счёт таймера не останавливается (без указания параметров будет выключен канал А).
TimerN.pause() - приостановить счёт таймера, не сбрасывая счётчик
TimerN.resume() - продолжить счёт после паузы
TimerN.stop() - остановить счёт и сбросить счётчик
TimerN.restart() - перезапустить таймер (сбросить счётчик)
TimerN.setDefault() - установить параметры таймера по умолчанию ("Ардуино-умолчания")
TimerN.outputEnable(канал, режим) - канал: включить выход таймера CHANNEL_A или CHANNEL_B (+ CHANNEL_C у Mega2560). Режим: TOGGLE_PIN, CLEAR_PIN, SET_PIN (переключить/выключить/включить пин по прерыванию)
TimerN.outputDisable(канал) - отключить выход таймера CHANNEL_A или CHANNEL_B (+ CHANNEL_C у Mega2560, см. такблицу таймеров)
TimerN.outputState(канал, состояние) - сменить состояние канала: HIGH / LOW

Прерывание
ISR(TIMERn_A) {
// ваш код
}
```
</p>
</details>

---

<a id="GyverTimer012"></a>
### GyverTimer012 v1.0 [СКАЧАТЬ](https://github.com/AlexGyver/GyverLibs/releases/download/GyverTimer012/GyverTimer012.zip), [ДОКУМЕНТАЦИЯ](https://alexgyver.ru/gyvertimer012/)
**Библиотека устарела. Используйте GyverTimers**  
Лёгкая библиотека для управления всеми тремя таймерами ATmega328
- Генерация прерываний по таймерам 0/1/2
- Настройка частоты или периода прерываний
- Остановка, сброс и продолжение счёта
#### Методы и функции библиотеки
<details>
<summary>РАЗВЕРНУТЬ</summary>
<p>
Смотри примеры в папке examples!

```C
// N - номер таймера (0, 1 или 2)
void timerN_ISR(void (*isr)());         // подключить прерывание
void timerN_setPeriod(uint32_t time);   // установить период (мкс)
void timerN_setFrequency(uint32_t hz);  // установить частоту (Гц)
void timerN_start(void);                // запустить
void timerN_stop(void);                 // остановить
void timerN_resume(void);               // продолжить
void timerN_restart(void);              // перезапустить
```
</p>
</details>

---

<a id="buildTime"></a>
### buildTime v1.0 [СКАЧАТЬ](https://github.com/AlexGyver/GyverLibs/releases/download/buildTime/buildTime.zip), [ДОКУМЕНТАЦИЯ](https://alexgyver.ru/buildtime/)
Лёгкая библиотека для получения даты и времени компиляции скетча
- Написана на "дефайнах", очень лёгкая и быстрая

#### Методы и функции библиотеки
<details>
<summary>РАЗВЕРНУТЬ</summary>
<p>
Смотри пример в папке examples!

```C
// Библиотека содержит макро-функции, возвращающие цифру
BUILD_YEAR	- год
BUILD_MONTH	- месяц
BUILD_DAY	- день
BUILD_HOUR	- час
BUILD_MIN	- минута
BUILD_SEC	- секунда
```
</p>
</details>

---

<a id="directADC"></a>
### directADC v1.1 [СКАЧАТЬ](https://github.com/AlexGyver/GyverLibs/releases/download/directADC/directADC.zip), [ДОКУМЕНТАЦИЯ](https://alexgyver.ru/directadc/)
Библиотека для расширенного (ручного) управления АЦП и компаратором ATmega328
- Функции библиотеки позволяют получить доступ ко всем возможностям и режимам работы с АЦП и компаратором
- Ничего не урезано и не упрощено, доступен весь описанный в даташите функционал
- Смотрите примеры
#### Методы и функции библиотеки
<details>
<summary>РАЗВЕРНУТЬ</summary>
<p>
Смотри примеры в папке examples!

```C
void setAnalogMux(ADC_modes mux);           // Аналоговый вход (ADC_A0-ADC_A7)/ термодатчик (ADC_SENSOR)/ 1.1V (ADC_1V1)/ ADC_GND (default: ADC_A0)
void ADC_enable(void);                      // Включить АЦП 
void ADC_disable(void);                     // Выключить АЦП (default)
void ADC_setPrescaler(byte prescl);         // Выбрать делитель частоты АЦП (2, 4, 8, 16, 32, 64, 128) // (default: 2)
void ADC_setReference(ADC_modes ref);       // Выбрать источник опорного напряжения АЦП (ADC_1V1, ADC_AREF, ADC_VCC) // (default: ADC_AREF)
void ADC_autoTriggerEnable(ADC_modes trig); // Включить автозапуск АЦП и выбрать событие (FREE_RUN, ANALOG_COMP, ADC_INT0, TIMER0_COMPA, TIMER0_OVF, TIMER1_COMPB, TIMER1_OVF)
void ADC_autoTriggerDisable(void);          // Выключить автозапуск АЦП // (default)
void ADC_attachInterrupt(void (*isr)());    // Включить прерывание готовности АЦП и выбрать функцию, которая будет при этом выполняться
void ADC_detachInterrupt(void);             // Выключить прерывание готовности АЦП // (default) 
void ADC_startConvert(void);                // Ручной запуск преобразования
unsigned int ADC_read(void);                // Прочитать значение регистров АЦП (Вызов до окончания преобразования вернет неверный результат)
boolean ADC_available(void);                // Проверить готовность преобразования АЦП
unsigned int ADC_readWhenAvailable(void);   // Дождаться окончания текущего преобразования и вернуть результат
void ACOMP_attachInterrupt(void (*isr)(), ADC_modes source); // Включить прерывание компаратора и выбрать при каком событии оно будет вызвано (FALLING_TRIGGER, RISING_TRIGGER, CHANGE_TRIGGER)
void ACOMP_detachInterrupt(void);           // Выключить прерывание компаратора // (default)
void ACOMP_enable(void);                    // Включить компаратор // (default: Включен)
void ACOMP_disable(void);                   // Принудительно выключить компаратор
boolean ACOMP_read(void);                   // Прочитать значение на выходе компаратора 
void ACOMP_setPositiveInput(ADC_modes in);  // Настроить куда подкл +Вход компаратора (ADC_1V1, ADC_AIN0) (default: ADC_AIN0 - pin 6) 
void ACOMP_setNegativeInput(ADC_modes in);  // Настроить куда подкл -Вход компаратора (ADC_AIN1, ANALOG_MUX) (default: ADC_AIN1 - pin 7)
```
</p>
</details>

---

<a id="directTimers"></a>
### directTimers v1.0 [СКАЧАТЬ](https://github.com/AlexGyver/GyverLibs/releases/download/directTimers/directTimers.zip), [ДОКУМЕНТАЦИЯ](https://alexgyver.ru/directtimers/)
Библиотека для расширенного (ручного) управления таймерами ATmega328
- Функции библиотеки позволяют получить доступ ко всем возможностям и режимам работы с таймерами/счётчиками + прерывания watchdog
- Ничего не урезано и не упрощено, доступен весь описанный в даташите функционал
- Смотрите примеры
#### Методы и функции библиотеки
<details>
<summary>РАЗВЕРНУТЬ</summary>
<p>
Смотри примеры в папке examples!

```C
// n - номер таймера (0, 1 или 2)
void TIMERn_COMPA_attachInterrupt(void (*isr)());
void TIMERn_COMPB_attachInterrupt(void (*isr)());
void TIMERn_COMPA_detachInterrupt(void);
void TIMERn_COMPB_detachInterrupt(void);
void TIMERn_setClock(byte clk);
void TIMERn_setMode(byte mode);
void TIMERn_COMPA_mode(byte mode);
void TIMERn_COMPB_mode(byte mode);
byte TIMERn_getCounter(void);
void TIMERn_setCounter(byte value);
void TIMERn_COMPA_setValue(byte value);
void TIMERn_COMPB_setValue(byte value);

// прерывания watchdog
void WDT_attachInterrupt(void (*isr)(),int prescaler);
void WDT_detachInterrupt(void);
```
</p>
</details>

---

<a id="GyverPWM"></a>
### GyverPWM v1.34 [СКАЧАТЬ](https://github.com/AlexGyver/GyverLibs/releases/download/GyverPWM/GyverPWM.zip), [ДОКУМЕНТАЦИЯ](https://alexgyver.ru/gyverpwm/)
Библиотека для расширенной генерации ШИМ на Arduino UNO/NANO/MINI (ATmega328p)
- Генерация ШИМ любой частоты от 250 Гц до 200 кГц на пинах D3, D5, D9 и D10
- Изменение разрядности ШИМ (пины D3 и D5: 4-8 бит, пины D9 и D10: 4-16 бит) с максимальной частотой (пример: 4 бита - 1 МГц ШИМ) на пинах D3, D5, D9 и D10. Или 12 бит ШИМ на частоте 4 кГЦ (пины 9 и 10)
- Несколько функций для фиксированных частот 16/20 кГц (на пинах D3 и D5 8 бит, на пинах D9 и D10 - 10 бит)
- Генерация меандра на пине D9 с частотой от 2 Гц до 8 МГц с максимальной точностью
- Несколько частот на выбор (30 Гц - 62 кГц) для генерации ШИМ стандартной analogWrite на всех ШИМ пинах (D3, D5, D6, D9, D10, D11)
- Изменение режима работы ШИМ (Fast/Correct PWM)
#### Методы и функции библиотеки
<details>
<summary>РАЗВЕРНУТЬ</summary>
<p>
Смотри примеры в папке examples!

```C
// ============== Функции для расширенной генерации ШИМ сигнала ==============

// Данные функции убирают один ШИМ выход у 8-ми битных таймеров, оставляя нам ШИМ пины D3, D5, D9 и D10 на ATmega328

void PWM_frequency(byte pin, long freq, modes correct);
/*	PWM_freqency(пин, частота, режим) - запустить ШИМ с выбранной частотой
	- Пины: D3 (таймер 2), D5 (таймер 0 - сломает millis/delay), D9 и D10 (таймер 1)
	- Режим: 0 (FAST_PWM), 1 (CORRECT_PWM)
	- Частота: 250-200'000 Гц для всех таймеров
	- Для изменения заполнения используй PWM_set
		- Разрядность в этом режиме приведена к 8 битам, на деле шаги изменения разные!
*/

void PWM_resolution(byte pin, byte res, modes correct);
/*	PWM_resolution(пин, разрядность, режим) - запустить ШИМ с выбранной разрядностью
	- Пины: D3 (таймер 2), D5 (таймер 0 - сломает millis/delay), D9 и D10 (таймер 1)
	- Режим: 0 (FAST_PWM), 1 (CORRECT_PWM)
	- Разрешение: D3 (4-8 бит), D5 (4-8 бит), D9 и D10 (4-16 бит)
	- Частота в этом режиме выбирается автоматически максимальная согласно возможностям таймера (см. таблицу)
	- Для изменения заполнения используй PWM_set
		- Пределы заполнения для разной разрядности указаны в таблице
*/

void PWM_set(byte pin, unsigned int duty);
/*	PWM_set(пин, заполнение) - изменить заполнение на выбранном пине
	- Пин: D3, D5, D6, D9, D10, D11
	- Заполнение: зависит от разрешения и режима (см. таблицу)
		- При использовании PWM_frequency разрядность составляет 8 бит (0-255)
		- При использовании PWM_resolution макс. значение заполнения равно (2^разрядность - 1), также смотри таблицу
*/

void PWM_detach(byte pin);		// отключает ШИМ на выбранном пине (позволяет использовать digital Read/Write)
void PWM_attach(byte pin);		// подключает ШИМ на выбранном пине (с последними настройками)
void PWM_default(byte pin);		// сброс настроек соответствующего пину таймера на "стандартные" для Arduino

void PWM_16KHZ_D3(byte duty);
/* 	Запуск ШИМ с частотой 16 кГц на пине D3
	- Отменяет настройки PWM_frequency/PWM_resolution
	- Разрядность приведена к 8 битам (заполнение 0-255)
	- Заполнение меняет сама (не нужно вызывать PWM_set) */

void PWM_20KHZ_D3(byte duty);
/* 	Запуск ШИМ с частотой 20 кГц на пине D3
	- Отменяет настройки PWM_frequency/PWM_resolution
	- Разрядность приведена к 8 битам (заполнение 0-255)
	- Заполнение меняет сама (не нужно вызывать PWM_set) */

void PWM_16KHZ_D5(byte duty);
/* 	Запуск ШИМ с частотой 16 кГц на пине D5
	- Отменяет настройки PWM_frequency/PWM_resolution
	- Разрядность приведена к 8 битам (заполнение 0-255)
	- Заполнение меняет сама (не нужно вызывать PWM_set) */

void PWM_20KHZ_D5(byte duty);
/* 	Запуск ШИМ с частотой 20 кГц на пине D5
	- Отменяет настройки PWM_frequency/PWM_resolution
	- Разрядность приведена к 8 битам (заполнение 0-255)
	- Заполнение меняет сама (не нужно вызывать PWM_set) */

void PWM_16KHZ_D9(int duty);
/* 	Запуск ШИМ с частотой 16 кГц (15.6 кГц) на пине D9
	- Отменяет настройки PWM_frequency/PWM_resolution
	- Разрядность ровно 10 бит (заполнение 0-1023)
	- Заполнение меняет сама (не нужно вызывать PWM_set) */

void PWM_20KHZ_D9(int duty);
/* 	Запуск ШИМ с частотой 20 кГц на пине D9
	- Отменяет настройки PWM_frequency/PWM_resolution
	- Разрядность приведена к 10 битам (заполнение 0-1023)
	- Заполнение меняет сама (не нужно вызывать PWM_set) */

void PWM_16KHZ_D10(int duty);
/* 	Запуск ШИМ с частотой 16 кГц (15.6 кГц) на пине D10
	- Отменяет настройки PWM_frequency/PWM_resolution
	- Разрядность ровно 10 бит (заполнение 0-1023)
	- Заполнение меняет сама (не нужно вызывать PWM_set) */

void PWM_20KHZ_D10(int duty);
/* 	Запуск ШИМ с частотой 20 кГц на пине D10
	- Отменяет настройки PWM_frequency/PWM_resolution
	- Разрядность приведена к 10 битам (заполнение 0-1023)
	- Заполнение меняет сама (не нужно вызывать PWM_set) */
/*
	============= Таблица №1 частот для расширенной генерации ШИМ (PWM_resolution) =============
 _________________________________________________________________________________________________________________________
|Разрядность, бит   |4      |5      |6      |7      |8     |9       |10      |11     |12     |13     |14    |15    |16    |
|___________________|_______|_______|_______|_______|______|________|________|_______|_______|_______|______|______|______|
|Макс. значение duty|15     |31     |63     |127    |255   |511     |1023    |2047   |4095   |8191   |16383 |32767 |65535 |
|___________________|_______|_______|_______|_______|______|________|________|_______|_______|_______|______|______|______|
|Fast   | Пины 3, 5 |1 МГц  |516 кГц|254 кГц|126 кГц|63 кГц|-       |-       |-      |-      |-      |-     |-     |-     |
|PWM    | 9, 10     |1 МГц  |516 кГц|254 кГц|126 кГц|63 кГц|31.2 кГц|15.6 кГц|7.8 кГц|3.9 кГц|1.9 кГц|980 Гц|488 Гц|244 Гц|
|_______|___________|_______|_______|_______|_______|______|________|________|_______|_______|_______|______|______|______|
|Correct| Пины 3, 5 |571 кГц|266 кГц|129 кГц|63 кГц |32 кГц|-       |-       |-      |-      |-      |-     |-     |-     |
|PWM    | 9, 10     |571 кГц|266 кГц|129 кГц|63 кГц |32 кГц|15.7 кГц|7.8 кГц |3.9 кГц|1.9 кГц|976 Гц |488 Гц|244 Гц|122 Гц|
|_______|___________|_______|_______|_______|_______|______|________|________|_______|_______|_______|______|______|______|
*/

// ============ Функции для настройки стандартной генерации ШИМ сигнала (analogWrite) ============

// Данные функции НЕ убирают один ШИМ выход у 8-ми битных таймеров, можно использовать все 6 ШИМ пинов с настроенной частотой! См. таблицу.

void PWM_prescaler(byte pin, byte mode);
/*	PWM_prescaler(пин, режим) - установить предделитель таймера (меняет частоту ШИМ)
	- Пин: D3, D5, D6, D9, D10, D11
	- Режим: 1-7, см. таблицу частот
*/

void PWM_mode(byte pin, byte mode);
/*	PWM_mode(пин, режим) - установить режим генерации ШИМ
	- Пин: D3, D5, D6, D9, D10, D11
	- Режим: 0 - FastPWM, 1 - Phase-correct, см. таблицу частот
*/

void PWM_TMR1_8BIT();	// Установить таймер 1 (ШИМ на D9 и D10) в режим 8 бит. См. таблицу частот

void PWM_TMR1_10BIT();	// Установить таймер 1 (ШИМ на D9 и D10) в режим 10 бит. См. таблицу частот

/*
	========== Таблица №2 частот для стандартной генерации ШИМ (PWM_prescaler) ==========
	
	Timer 0 по умолчанию работает в режиме Fast PWM 
	Timer 1 и 2 по умолчанию работают в режиме Phase-correct
 _______________________________________________________________________________________________
|       | Timer0 (пины 5 и 6) 8 bit | Timer 1 (пины 9 и 10) 10 bit  | Timer2 (пины 3 и 11) 8 bit|
|       | Timer1 (пины 9 и 10) 8 bit|                               |                           |
|       |___________________________|_______________________________|___________________________|
|mode   | Phase-correct | Fast PWM  | Phase-correct     | Fast PWM  | Phase-correct | Fast PWM  |
|_______|_______________|___________|___________________|___________|_______________|___________|
|1      | 31.4 kHz      | 62.5 kHz  | 7.8 kHz           | 15.6 kHz  | 31.4 kHz      | 62.5 kHz  |
|2      | 4 kHz         | 7.8 kHz   | 977 Hz            | 2 kHz     | 4 kHz         | 8 kHz     |
|3      | 490 Hz        | 976 Hz    | 122 Hz            | 244 Hz    | 980 Hz        | 2 kHz     |
|4      | 122 Hz        | 244 Hz    | 30 Hz             | 61 Hz     | 490 Hz        | 980 Hz    |
|5      | 30 Hz         | 61 Hz     | 7.6 Hz            | 15 Hz     | 245 Hz        | 490 Hz    |
|6      | -             | -         | -                 | -         | 122 Hz        | 244 Hz    |
|7      | -             | -         | -                 | -         | 30 Hz         | 60 Hz     |
|_______|_______________|___________|___________________|___________|_______________|___________|
*/
```
</p>
</details>

---

<a id="GyverWDT"></a>
### GyverWDT v2.1 [СКАЧАТЬ](https://github.com/AlexGyver/GyverLibs/releases/download/GyverWDT/GyverWDT.zip), [ДОКУМЕНТАЦИЯ](https://alexgyver.ru/gyverwdt/)
Библиотека для расширенной и удобной работы с watchdog
- Перезагрузка по прерыванию
- Вызов обычных прерываний
- Настройка периода работы
#### Методы и функции библиотеки
<details>
<summary>РАЗВЕРНУТЬ</summary>
<p>
Смотри примеры в папке examples!

```C
void reset(void);                               // сброс
void disable(void);                             // отключить WDT
void enable(uint8_t mode, uint8_t prescaler);   // включить WDT с настройками
// mode:
// RESET_MODE - сброс при зависании (при тайм-ауте WDT)
// INTERRUPT_MODE - прерывание при зависании (при тайм-ауте WDT)
// INTERRUPT_RESET_MODE - первый таймаут - прерывание, второй - сброс
// prescaler:
// WDT_PRESCALER_2, WDT_PRESCALER_4... WDT_PRESCALER_1024
```
</p>
</details>

---

<a id="ServoSmooth"></a>
### ServoSmooth v3.0 [СКАЧАТЬ](https://github.com/AlexGyver/GyverLibs/releases/download/ServoSmooth/ServoSmooth.zip), [ДОКУМЕНТАЦИЯ](https://alexgyver.ru/servosmooth/)
Библиотека для плавного управления сервоприводами
- Является дополнением к стандартной библиотеке Servo
- Также работает с расширителем серво PCA9685
- Настройка максимальной скорости сервопривода
- Настройка ускорения (разгон и торможение) сервопривода
- При использовании ESC и БК мотора получаем "плавный пуск" мотора
- Устанвока целевой позиции серво по углу (0-180) и длине импульса (500-2400)
- Автоматическое отключение привода по таймауту неактивности и включение при изменении позиции (настраивается)
#### Методы и функции библиотеки
<details>
<summary>РАЗВЕРНУТЬ</summary>
<p>
Смотри примеры в папке examples!

```C
void write(uint16_t angle);                 // аналог метода из библиотеки Servo
void writeMicroseconds(uint16_t angle);     // аналог метода из библиотеки Servo
void attach(uint8_t pin);                   // аналог метода из библиотеки Servo
void attach(uint8_t pin, int min, int max); // аналог метода из библиотеки Servo. min по умолч. 500, max 2400
void detach();                              // аналог метода detach из библиотеки Servo
void start();                               // attach + разрешает работу tick
void stop();                                // detach + запрещает работу tick
		
boolean tick();                             // метод, управляющий сервой, должен опрашиваться как можно чаще.
                                            // Возвращает true, когда целевая позиция достигнута.
                                            // Имеет встроенный таймер с периодом SERVO_PERIOD

boolean tickManual();                       // метод, управляющий сервой, без встроенного таймера.
                                            // Возвращает true, когда целевая позиция достигнута

void setSpeed(int speed);                   // установка максимальной скорости (условные единицы, 0 - 200)
void setAccel(float accel);                 // установка ускорения (0.05 - 1). При значении 1 ускорение максимальное
void setTarget(int target);                 // установка целевой позиции в мкс (500 - 2400)
void setTargetDeg(int target);              // установка целевой позиции в градусах (0-180). Зависит от min и max
void setAutoDetach(boolean set);            // вкл/выкл автоматического отключения (detach) при достижении угла. По умолч. вкл
```
</p>
</details>

---

<a id="GyverFilters"></a>
### GyverFilters v2.0 [СКАЧАТЬ](https://github.com/AlexGyver/GyverLibs/releases/download/GyverFilters/GyverFilters.zip), [ДОКУМЕНТАЦИЯ](https://alexgyver.ru/gyverfilters/)
Библиотека с некоторыми удобными фильтрами для Arduino:
- GFilterRA - компактная альтернатива фильтра экспоненциальное бегущее среднее (Running Average)			
- GMedian3 - быстрый медианный фильтр 3-го порядка (отсекает выбросы)
- GMedian - медианный фильтр N-го порядка. Порядок настраивается в GyverFilters.h - MEDIAN_FILTER_SIZE
- GABfilter - альфа-бета фильтр (разновидность Калмана для одномерного случая)
- GKalman - упрощённый Калман для одномерного случая (на мой взгляд лучший из фильтров)
- GLinear - линейная аппроксимация методом наименьших квадратов для двух массивов
#### Методы и функции библиотеки
<details>
<summary>РАЗВЕРНУТЬ</summary>
<p>
Смотри примеры в папке examples!

```C
// ============== Бегущее среднее ==============
GFilterRA();                                // инициализация фильтра
GFilterRA(float coef, uint16_t interval);   // расширенная инициализация фильтра (коэффициент, шаг фильтрации)
void setCoef(float coef);                   // настройка коэффициента фильтрации (0.00 - 1.00). Чем меньше, тем плавнее
void setStep(uint16_t interval);            // установка шага фильтрации (мс). Чем меньше, тем резче фильтр
	
float filteredTime(int16_t value);          // возвращает фильтрованное значение с опорой на встроенный таймер	
float filtered(int16_t value);              // возвращает фильтрованное значение
	
float filteredTime(float value);            // возвращает фильтрованное значение с опорой на встроенный таймер	
float filtered(float value);                // возвращает фильтрованное значение

// ============== Медиана из трёх ==============
GMedian3();                     // инициализация фильтра
uint16_t filtered(uint16_t);    // возвращает фильтрованное значение

// ============== Медиана из MEDIAN_FILTER_SIZE (настраивается в GyverFilters.h) ==============
GMedian();                      // инициализация фильтра
uint16_t filtered(uint16_t);    // возвращает фильтрованное значение

// ============== Альфа-Бета фильтр ==============
GABfilter(float delta, float sigma_process, float sigma_noise);
// период дискретизации (измерений), process variation, noise variation
		
void setParameters(float delta, float sigma_process, float sigma_noise);
// период дискретизации (измерений), process variation, noise variation
		
float filtered(float value);      // возвращает фильтрованное значение

// ============== Упрощённый Калман ==============
GKalman(float mea_e, float est_e, float q);
// разброс измерения, разброс оценки, скорость изменения значений
		
GKalman(float mea_e, float q);
// разброс измерения, скорость изменения значений (разброс измерения принимается равным разбросу оценки)
		
void setParameters(float mea_e, float est_e, float q);
// разброс измерения, разброс оценки, скорость изменения значений
		
void setParameters(float mea_e, float q);
// разброс измерения, скорость изменения значений (разброс измерения принимается равным разбросу оценки)
		
float filtered(float value);     // возвращает фильтрованное значение

// ============== Линейная аппроксимация ==============
void compute(int *x_array, int *y_array, int arrSize);		// аппроксимировать
float getA();		// получить коэффициент А
float getB();		// получить коэффициент В
float getDelta();	// получить аппроксимированное изменение
```
</p>
</details>

---

<a id="GyverTimer"></a>
### GyverTimer v3.2 [СКАЧАТЬ](https://github.com/AlexGyver/GyverLibs/releases/download/GyverTimer/GyverTimer.zip), [ДОКУМЕНТАЦИЯ](https://alexgyver.ru/gyvertimer/)
GTimer - полноценный таймер на базе системных millis() / micros(), обеспечивающий удобную мультизадачность и работу с временем
- Миллисекундный и микросекундный таймер
- Два режима работы:
	- Режим интервала: таймер "срабатывает" каждый заданный интервал времени
	- Режим таймаута: таймер "срабатывает" один раз по истечении времени (до следующего перезапуска)
- Служебные функции:
	- Старт
	- Стоп
	- Сброс
	- Продолжить

#### Методы и функции библиотеки
<details>
<summary>РАЗВЕРНУТЬ</summary>
<p>
Смотри примеры в папке examples!

```C
GTimer(timerType type, uint32_t interval);  // объявление таймера с указанием типа и интервала (таймер не запущен, если не указывать)
void setInterval(uint32_t interval);    // установка интервала работы таймера (также запустит и сбросит таймер) - режим интервала
void setTimeout(uint32_t timeout);      // установка таймаута работы таймера (также запустит и сбросит таймер) - режим таймаута
boolean isReady();                      // возвращает true, когда пришло время
boolean isEnabled();                    // вернуть состояние таймера (остановлен/запущен)
void reset();                           // сброс таймера на установленный период работы
void start();                           // запустить/перезапустить (со сбросом счёта)
void stop();                            // остановить таймер (без сброса счёта)	
void resume();                          // продолжить (без сброса счёта)	
```
</p>
</details>

---

<a id="GyverHacks"></a>
### GyverHacks v2.10 [СКАЧАТЬ](https://github.com/AlexGyver/GyverLibs/releases/download/GyverHacks/GyverHacks.zip), [ДОКУМЕНТАЦИЯ](https://alexgyver.ru/gyverhacks/)
Библиотека с некоторыми удобными хаками для Arduino UNO/NANO/MINI (atmega328):
- Быстрые аналоги стандартных функций чтения/записи (v2.8 исправлен баг)
- Изменение частоты ШИМ пинов (3, 5, 6, 9, 10, 11)
- Установка ШИМ на пинах 9 и 10 в режим 10 бит (analogWrite 0-1023)
- Генерация ШИМ на ЛЮБОМ пине (частота ~150 Гц)
- Измерение напряжения питания + калибровка константы
- Перевод напряжения питания в проценты по графику разряда для разных типов АКБ
- Измерение температуры ядра
#### Методы и функции библиотеки
<details>
<summary>РАЗВЕРНУТЬ</summary>
<p>
Смотри примеры в папке examples!

```C
// ********************************************* Хаки с ШИМ *********************************************
void setPWMmode(byte pin, byte mode);   // установка режима ШИМ на пине 
                                        // пины 3, 5, 6, 9, 10, 11
                                        // режимы: 0 - FastPWM, 1 - Phase-correct PWM

void setPWMprescaler(uint8_t pin, uint16_t mode);
// установка частоты ШИМ на пине. Смотри таблицу частот в GyverHacks.h или примере PWMfreq!

void set8bitPWM();                 // установка ШИМ на пинах 9 и 10 в режим 8 бит (analogWrite 0-255) (по умолчанию)

void set10bitPWM();                // установка ШИМ на пинах 9 и 10 в режим 10 бит (analogWrite 0-1023)

void delayFix(uint32_t delayTime);                  // аналог delay для корректной работы с изменённой частотой ШИМ пинов 5 и 6
void delayMicrosecondsFix(uint32_t delayTime);      // аналог delayMicroseconds для корректной работы с изменённой частотой ШИМ пинов 5 и 6

uint32_t millisFix();                               // аналог millis для корректной работы с изменённой частотой ШИМ пинов 5 и 6
uint32_t microsFix();                               // аналог micros для корректной работы с изменённой частотой ШИМ пинов 5 и 6

void anyPWMinit(byte prescaler);
// инициализация ШИМ на любом пине
// prescaler: 4 - 311 Гц, 5 - 244 Гц, 6 - 122 Гц, 7 - 30 Гц
// если пинов много - понижайте частоту (20 пинов работают отлично на 6 режиме)													
													
void anyPWMpin(uint8_t pin);       // настроить ЛЮБОЙ пин для генерации ШИМ

void anyPWM(byte pin, byte duty);  // включить ШИМ на ЛЮБОМ пине (настроенном выше)

// ********************************************* Ускоряем функции *********************************************

void setPWM(uint8_t pin, uint8_t duty);      // быстрый аналог analogWrite (в 7 раз быстрее)

void setPin(uint8_t pin, uint8_t x);         // быстрый аналог digitalWrite (в 10 раз быстрее)

boolean readPin(uint8_t pin);                // быстрый аналог digitalRead (в 11 раз быстрее)

void setADCrate(byte mode);
// установка скорости работы АЦП (analogRead)
// mode 1: 3.04 мкс (частота оцифровки 329 000 кГц)
// mode 2: 4.72 мкс (частота оцифровки 210 000 кГц)
// mode 3: 8.04 мкс (частота оцифровки 125 000 кГц)
// mode 4: 15.12 мкс (частота оцифровки 66 100 кГц)
// mode 5: 28.04 мкс (частота оцифровки 35 600 кГц)
// mode 6: 56.04 мкс (частота оцифровки 17 800 кГц)													
// mode 7: 112 мкс (частота оцифровки 8 900 Гц)

// ********************************************* Точный вольтметр *********************************************

int getVCC();                      // возвращает опорное напряжение (напряжение питания)
                                   // константа по умолчанию равна 1100, нужно калибровать!
								
void constantWizard();             // помошник калибровки константы (смотри пример)

void restoreConstant(int adr);     // восстановить константу из памяти

void setConst(int new_const);      // установить константу вручную (по умолч. 1100)

int getConst();                    // вывести текущую константу

int getVoltage(uint8_t pin);       // измерить напряжение на пине с учётом опорного

// функции получения процента заряда из напряжения, линеаризованы вручную по графикам разряда АКБ
// использовать вот так: lithiumPercent(getVCC()); - ардуина питается от лития

byte lithiumPercent(int volts);    // возвращает процент заряда Li-ion аккумулятора (4,2-2,8 В)

byte alkaline3Percent(int volts);  // возвращает процент заряда 3х алкалиновых батареек (4,6-3,3 В)

byte nickel3Percent(int volts);    // возвращает процент заряда 3х Ni-Cd аккумуляторов (4.2-3.0 В)

byte nickel4Percent(int volts);    // возвращает процент заряда 4х Ni-Cd аккумуляторов (5.6-4.0В)

// функция для расчёта заряда батареи в процентах
// принимает напряжение в милливолльтах (volts), а также напряжения, при которых заряд равен 100, 80... 0%
byte mVtoPercent(int volts, int volt100, int volt80, int volt60, int volt40, int volt20, int volt0);

// ********************************************* Прочее *********************************************
float getTemp();		// получить примерную температуру ядра процессора (температура окружающей среды или температура внутри корпуса)

```
</p>
</details>

---

<a id="GyverMotor"></a>
### GyverMotor v2.1 [СКАЧАТЬ](https://github.com/AlexGyver/GyverLibs/releases/download/GyverMotor/GyverMotor.zip), [ДОКУМЕНТАЦИЯ](https://alexgyver.ru/gyvermotor/)
Библиотека для удобного управления моторчиками через драйвер полного моста для Arduino
- Контроль скорости и направления вращения
- Работа с 10 битным ШИМом
- Программный deadtime
- Отрицательные скорости
- Поддержка двух типов драйверов и реле
- Плавный пуск и изменение скорости
#### Методы и функции библиотеки
<details>
<summary>РАЗВЕРНУТЬ</summary>
<p>
Смотри примеры в папке examples!

```C
GMotor(driverType type, int8_t param1 = NC, int8_t param2 = NC, int8_t param3 = NC, int8_t param4 = NC);
// три варианта создания объекта в зависимости от драйвера:
// GMotor motor(DRIVER2WIRE, dig_pin, PWM_pin, (LOW/HIGH) )
// GMotor motor(DRIVER3WIRE, dig_pin_A, dig_pin_B, PWM_pin, (LOW/HIGH) )
// GMotor motor(RELAY2WIRE, dig_pin_A, dig_pin_B, (LOW/HIGH) )
	
// установка скорости 0-255 (8 бит) и 0-1023 (10 бит)
void setSpeed(int16_t duty);			
	
// сменить режим работы мотора:	
// FORWARD - вперёд
// BACKWARD - назад
// STOP - остановить
// BRAKE - активное торможение
// AUTO - подчиняется setSpeed (-255.. 255) и 0-1023 (10 бит)
void setMode(workMode mode);
	
// направление вращения	
// NORM - обычное
// REVERSE - обратное
void setDirection(dir direction);
	
// установить выход в 8 бит
void set8bitMode();		

// установить выход в 10 бит
void set10bitMode();					
	
// установить deadtime (в микросекундах). По умолч 0
void setDeadtime(uint16_t deadtime);	
	
// установить уровень драйвера (по умолч. HIGH)
void setLevel(int8_t level);			
	
// плавное изменение к указанной скорости
void smoothTick(int16_t duty);
	
// скорость изменения скорости
void setSmoothSpeed(uint8_t speed);

// внутренняя переменная скважности для отладки
int16_t _duty = 0;
```
</p>
</details>

---

<a id="AccelMotor"></a>
### AccelMotor v1.0 [СКАЧАТЬ](https://github.com/AlexGyver/GyverLibs/releases/download/AccelMotor/AccelMotor.zip), [ДОКУМЕНТАЦИЯ](https://alexgyver.ru/accelmotor/)
Библиотека для управления моторчиками через драйвер моста для Arduino с обратной связью
- Наследует все фишки из библиотеки GyverMotor (поддержка разных драйверов и режимов)
- Режим поддержания скорости с обратной связью
- Режим поворота на заданный угол с обратной связью
- Настраиваемые коэффициенты PID регулятора
- Ограничение ускорения и скорости
- Библиотека принимает любой тип обратной связи: энкодер, потенциометр, и т.д.
- Поддержка мотор-редукторов, настройка передаточного отношения энкодера
- Регулятор учитывает "мёртвую зону" мотора
- Все функции работают в градусах и "тиках" энкодера

#### Методы и функции библиотеки
<details>
<summary>РАЗВЕРНУТЬ</summary>
<p>
Смотри примеры в папке examples!

```C
// управляет мотором. Вызывать как можно чаще (внутри таймер с периодом dt)
// принимает текущее положение вала мотора (по счёту энкодера)
// возвращает true если мотор всё ещё движется к цели
bool tick(long pos);

// установка передаточного отношения редуктора и энкодера
// пример: если редуктор 1:30 - передаём в функцию 30
// пример: если редуктор 1:30 и энкодер на 12 тиков - передаём 30*12
void setRatio(float ratio);

// установка периода регулятора (рекомендуется 2-50 миллисекунд)
void setDt(int dt);

// установка и получение текущей позиции в тиках энкодера и градусах.
// setCurrent(pos) равносильна вызову tick(pos) и в принципе не нужна!
void setCurrent(long pos);
long getCurrent();
long getCurrentDeg();

// установка и получение целевой позиции в тиках энкодера и градусах
void setTarget(long pos);
void setTargetDeg(long pos);
long getTarget();
long getTargetDeg();

// установка максимальной скорости в тиках энкодера/секунду и градусах/секунду
void setMaxSpeed(int speed);
void setMaxSpeedDeg(int speed);

// установка ускорения тиках энкодера и градусах в секунду
void setAcceleration(float accel);
void setAccelerationDeg(float accel);

// установка и получение целевой скорости в тиках энкодера/секунду и градусах/секунду
void setTargetSpeed(int speed);
void setTargetSpeedDeg(int speed);
int getTargetSpeed();
int getTargetSpeedDeg();

// получить текущую скорость в тиках энкодера/секунду и градусах/секунду
int getSpeed();
int getSpeedDeg();

// получить текущий ШИМ сигнал (float из ПИД регулятора)
float getDuty();

// ручная установка режима работы
// IDLE_RUN - tick() не управляет мотором. Может использоваться для отладки
// ACCEL_POS - tick() работает в режиме плавного следования к целевому углу
// PID_POS - tick() работает в режиме резкого следования к целевому углу
// ACCEL_SPEED - tick() работает в режиме плавного поддержания скорости (с заданным ускорением)
// PID_SPEED - tick() работает в режиме поддержания скорости по ПИД регулятору
void setRunMode(runMode mode);

// возвращает true, если вал мотора заблокирован, а сигнал подаётся
bool isBlocked();

// коэффициенты ПИД регулятора
// пропорциональный - от него зависит агрессивность управления, нужно увеличивать kp
// при увеличении нагрузки на вал, чтобы регулятор подавал больший управляющий ШИМ сигнал
float kp = 2.0;// (знач. по умолчанию)

// интегральный - позволяет нивелировать ошибку со временем, имеет накопительный эффект
float ki = 0.9;// (знач. по умолчанию)

// дифференциальный. Позволяет чуть сгладить рывки, но при большом значении
// сам становится причиной рывков и раскачки системы!
float kd = 0.1;// (знач. по умолчанию)

// установить зону остановки мотора для режима стабилизации позиции (по умолч. 8)
void setStopZone(int zone);
```
</p>
</details>

---

<a id="GyverRGB"></a>
### GyverRGB v1.16 [СКАЧАТЬ](https://github.com/AlexGyver/GyverLibs/releases/download/GyverRGB/GyverRGB.zip), [ДОКУМЕНТАЦИЯ](https://alexgyver.ru/gyverrgb-lib/)
Библиотека для удобного управления RGB светодиодами и лентами для Arduino
- 1530 значений для colorWheel
- Работа в пространстве RGB
- Работа в пространстве HSV
- Установка цвета в формате HEX
- Установка цветовой температуры
- 16 предустановленных цветов
  Доступные цвета для setHEX
	+ WHITE   - белый
	+ SILVER  - серебро
	+ GRAY    - серый
	+ BLACK   - чёрный
	+ RED     - красный
	+ MAROON  - бордовый
	+ YELLOW  - жёлтый
	+ OLIVE   - олива
	+ LIME    - лайм
	+ GREEN   - зелёный
	+ AQUA    - аква
	+ TEAL    - цвет головы утки чирка https://odesign.ru/teal-color/
	+ BLUE    - голубой
	+ NAVY    - тёмно-синий
	+ PINK    - розовый
	+ PURPLE  - пурпурный
- Настройка полярности ШИМ	
- Функция плавной смены цвета
- Ограничение тока (по расчёту)
- Регулировка общей яркости
- Поддержание яркости LED ленты по мере разряда АКБ
- Возможность управления 6-ю RGB диодами/лентами с одной Arduino (встроенный генератор ШИМ на ВСЕХ 20 пинах atmega328)
- Режим с настройкой частоты ШИМ
- Матрица коррекции LUT
- Коррекция по минимальному сигналу ШИМ
- Гамма-коррекция яркости

#### Методы и функции библиотеки
<details>
<summary>РАЗВЕРНУТЬ</summary>
<p>
Смотри примеры в папке examples!

```C
// объявление
GRGB(uint8_t rpin, uint8_t gpin, uint8_t bpin);

// объявление с выбором режима генерации ШИМ (NORM_PWM / ANY_PWM)
// NORM_PWM - дефолтные ШИМ пины (3, 5, 6, 9, 10, 11 для UNO/NANO/MINI)
// ANY_PWM - любой пин делается ШИМ пином (частота ~150 Гц). Подробности в библиотеке GyverHacks
GRGB(uint8_t rpin, uint8_t gpin, uint8_t bpin, boolean pwmmode);

// NORMAL / REVERSE - направление ШИМ
// общий катод - NORMAL
// общий анод - REVERSE
void setDirection(boolean direction);

// установка ограничения по току: 
// numLeds - количество светодиодов
// vcc - напряжение питания в милливольтах
// maxCur - максимальный ток
void setMaxCurrent(uint16_t numLeds, float vcc, int maxCur);
																	
void setBrightness(byte bright);                     // установка яркости (0-255)
void constantBrightTick(int minVolts, int vcc);      // корректировка под напряжение питания
void gammaTick(int vcc);                             // корректировка красного цвета при падении напряжения питания

void setHEX(uint32_t color);                         // установка цвета в формате HEX (вида 0x808080 )
void setRGB(uint8_t r, uint8_t g, uint8_t b);        // установка цвета в пространстве RGB (каждый цвет 0-255)
void setHSV(uint8_t h, uint8_t s, uint8_t v);        // установка цвета в пространстве HSV (каждая велиична 0-255)
void setHSV_fast(uint8_t h, uint8_t s, uint8_t v);   // более быстрый, но менее красивый вариант предыдущей функции
void setKelvin(int16_t temperature);                 // установить цвет как температуру в Кельвинах (от 1000 до 10'000 - от красного к синему)
void colorWheel(int color);                          // установить цвет (0 - 1530). Максимально широкая палитра ярких цветов (смеси RGB)
	
// плавно изменить текущий цвет к новому за вермя fadeTime в миллисекундах
// для HEX цвета
void fadeTo(uint32_t newColor, uint16_t fadeTime);
	
// для R G B
void fadeTo(uint8_t new_r, uint8_t new_g, uint8_t new_b, uint16_t fadeTime);
```
</p>
</details>

---

<a id="GyverTM1637"></a>
### GyverTM1637 v1.4 [СКАЧАТЬ](https://github.com/AlexGyver/GyverLibs/releases/download/GyverTM1637/GyverTM1637.zip), [ДОКУМЕНТАЦИЯ](https://alexgyver.ru/tm1637_display/)
Бибилотека для 7 сегментного дисплея на чипе TM1637 с кучей приколюх
- Вывод цифр массивом или прицельно
- Вывод букв из списка доступных (листай ниже) массивом или прицельно
- Отдельная функция вывода часов и минут (часы без нуля слева, минуты с нулём) 3 разных эффекта
- Вывод числа от -999 до 9999 с учётом знака
- Готовая функция бегущей строки
- Функции смены яркости и состояния двоеточия автоматически обновляют дисплей
- Функция обновления значения с эффектом вертикальной прокрутки
- Функция обновления значения с эффектом скручивания (лучше один раз увидеть)

#### Методы и функции библиотеки
<details>
<summary>РАЗВЕРНУТЬ</summary>
<p>
Смотри примеры в папке examples!

```C
GyverTM1637(uint8_t clk, uint8_t dio);                                  // объявление и инициализация
	
void display(uint8_t DispData[]);                                       // выводит цифры массивом по ячейкам. От 0 до 9 (byte values[] = {3, 5, 9, 0}; )
void display(uint8_t BitAddr, int8_t DispData);                         // выводит цифру DispData в указанную ячейку дисплея BitAddr
void display(uint8_t bit0, uint8_t bit1, uint8_t bit2, uint8_t bit3);   // если лень создавать массив, выводит цифры в ячейки
	
void displayByte(uint8_t DispData[]);                                   // выводит байт вида 0xe6 и буквы-константы вида _a , _b .... массивом
void displayByte(uint8_t BitAddr, int8_t DispData);                     // выводит байт вида 0xe6 и буквы-константы вида _a , _b .... в ячейку
void displayByte(uint8_t bit0, uint8_t bit1, uint8_t bit2, uint8_t bit3);  // если лень создавать массив, выводит байты в ячейки
	
void displayClock(uint8_t hrs, uint8_t mins);                           // выводит часы и минуты
void displayClockScroll(uint8_t hrs, uint8_t mins, int delayms);        // выводит часы и минуты с эффектом прокрутки
void displayClockTwist(uint8_t hrs, uint8_t mins, int delayms);         // выводит часы и минуты с эффектом скрутки
	
void displayInt(int value);                                             // выводит число от -999 до 9999 (да, со знаком минус)
	
void runningString(int8_t DispData[], byte amount, int delayMs);        // бегущая строка (array, sizeof(array), задержка в мс)
	
void clear(void);                                                       // очистить дисплей
	
void point(boolean PointFlag);                                          // вкл / выкл точку (POINT_ON / POINT_OFF)

void brightness(uint8_t bright, uint8_t = 0x40, uint8_t = 0xc0);        // яркость 0 - 7	
	
void scroll(uint8_t BitAddr, int8_t DispData, int delayms);             // обновить значение прокруткой (адрес, ЦИФРА, задержка в мс)
void scroll(int8_t DispData[], int delayms);                            // обновить значение прокруткой (массив ЦИФР, задержка в мс)
void scroll(uint8_t bit0, uint8_t bit1, uint8_t bit2, uint8_t bit3, int delayms);   // прокрутка посимвольно
void scrollByte(uint8_t BitAddr, int8_t DispData, int delayms);         // обновить значение прокруткой (адрес, БАЙТ, задержка в мс)
void scrollByte(int8_t DispData[], int delayms);                        // обновить значение прокруткой (массив БАЙТ, задержка в мс)
void scrollByte(uint8_t bit0, uint8_t bit1, uint8_t bit2, uint8_t bit3, int delayms);  // прокрутка посимвольно
	
void twist(uint8_t BitAddr, int8_t DispData, int delayms);              // обновить значение скручиванием (адрес, ЦИФРА, задержка в мс)
void twist(int8_t DispData[], int delayms);                             // обновить значение скручиванием (массив ЦИФР, задержка в мс)
void twist(uint8_t bit0, uint8_t bit1, uint8_t bit2, uint8_t bit3, int delayms);   // скрутка посимвольно
void twistByte(uint8_t BitAddr, int8_t DispData, int delayms);          // обновить значение скручиванием (адрес, БАЙТ, задержка в мс)
void twistByte(int8_t DispData[], int delayms);                         // обновить значение скручиванием (массив БАЙТ, задержка в мс)
void twistByte(uint8_t bit0, uint8_t bit1, uint8_t bit2, uint8_t bit3, int delayms);  // скрутка посимвольно

```
</p>
</details>

---

<a id="GyverPID"></a>
### GyverPID v2.4 [СКАЧАТЬ](https://github.com/AlexGyver/GyverLibs/releases/download/GyverPID/GyverPID.zip), [ДОКУМЕНТАЦИЯ](https://alexgyver.ru/gyverpid/)
Библиотека классического PID регулятора для Arduino
- Время одного расчёта около 70 мкс
- Режим работы по величине или по её изменению (для интегрирующих процессов)
- Возвращает результат по встроенному таймеру или в ручном режиме


#### Методы и функции библиотеки
<details>
<summary>РАЗВЕРНУТЬ</summary>
<p>
Смотри примеры в папке examples!

```C
==== datatype это float или int, в зависимости от выбранного (см. пример integer_calc) ====

GyverPID();
GyverPID(float new_kp, float new_ki, float new_kd, int16_t new_dt = 100);		// kp, ki, kd, dt
	
datatype setpoint = 0;		// заданная величина, которую должен поддерживать регулятор
float input = 0;			// сигнал с датчика (например температура, которую мы регулируем)
datatype output = 0;		// выход с регулятора на управляющее устройство (например величина ШИМ или угол поворота серво)
	
datatype getResult();												// возвращает новое значение при вызове (если используем свой таймер с периодом dt!)	
datatype getResult(datatype new_setpoint, datatype new_input);		// принимает setpoint и input
	
datatype getResultTimer();											// возвращает новое значение не ранее, чем через dt миллисекунд (встроенный таймер с периодом dt)
datatype getResultTimer(datatype new_setpoint, datatype new_input);	// тож самое, но принимает setpoint и input
	
void setDirection(uint8_t direction);						// направление регулирования: NORMAL (0) или REVERSE (1)
void setMode(uint8_t mode);									// режим: работа по входной ошибке ON_ERROR (0) или по изменению ON_RATE (1)
void setLimits(int min_output, int max_output);				// лимит выходной величины (например для ШИМ ставим 0-255)
void setDt(int16_t new_dt);									// установка времени дискретизации (для getResultTimer)
void tune(float new_kp, float new_ki, float new_kd);		// перенастройка коэффициентов (П, И, Д)

```
</p>
</details>

---

<a id="GyverRelay"></a>
### GyverRelay v2.1 [СКАЧАТЬ](https://github.com/AlexGyver/GyverLibs/releases/download/GyverRelay/GyverRelay.zip), [ДОКУМЕНТАЦИЯ](https://alexgyver.ru/gyverrelay/)
Библиотека классического релейного регулятора для Arduino
- Обратная связь по скорости изменения величины
- Настройка гистерезиса, коэффициента усиления ОС, направления регулирования
- Возвращает результат по встроенному таймеру или в ручном режиме

#### Методы и функции библиотеки
<details>
<summary>РАЗВЕРНУТЬ</summary>
<p>
Смотри примеры в папке examples!

```C
// принимает установку, ширину гистерезиса, направление (NORMAL, REVERSE)
// NORMAL - включаем нагрузку при переходе через значение снизу (пример: охлаждение)
// REVERSE - включаем нагрузку при переходе через значение сверху (пример: нагрев)
GyverRelay(float new_setpoint, float new_hysteresis, boolean direction);
GyverRelay();

// расчёт возвращает состояние для управляющего устройства (реле, транзистор) (1 вкл, 0 выкл)
boolean getResult();                        // расчёт
boolean getResult(float new_input);         // расчёт, принимает текущую величину с датчика
boolean getResultTimer();                   // расчёт по встроенному таймеру
boolean getResultTimer(float new_input);    // расчёт, принимает текущую величину с датчика (+ по встроенному таймеру)
	
void setDirection(boolean);                 // направление регулирования (NORMAL, REVERSE)
	
float input;                        // сигнал с датчика (например температура, которую мы регулируем)
float setpoint;                     // заданная величина, которую должен поддерживать регулятор (температура)
	
float signal;                       // сигнал (для отладки)
float hysteresis;                   // ширина гистерезиса (половина в минус, половина в плюс)
float k = 0;                        // коэффициент усиления	по скорости (по умолч. 0)
float rate;                         // скорость изменения величины (производная)
int16_t sampleTime = 1000;          // время итерации, мс (по умолч. секунда)
```
</p>
</details>

---

<a id="GyverRTOS"></a>
### GyverRTOS v1.0 [СКАЧАТЬ](https://github.com/AlexGyver/GyverLibs/releases/download/GyverRTOS/GyverRTOS.zip)
Система реального времени для Arduino: максимальное энергосбережение и мультизадачность
- Во время сна функция millis() не работает, вместо неё используется переменная mainTimer, которая автоматически увеличивается при каждом пробуждении на время сна (SLEEP_PERIOD) В ХОЛОСТОМ РЕЖИМЕ
- Выполнение функций занимает время, поэтому ЕСЛИ ВЫПОЛНЯЕТСЯ ЗАДАЧА, время выполнения тоже автоматически суммируется в mainTimer
- МЫ создаём несколько функций с разным периодом выполнения (задачи)
- Настраиваем период пробуждения системы (минимально 15 мс)
Далее всё автоматически:
- Рассчитывается время до выполнения самой "ближней" задачи
- Система периодически просыпается и считает таймеры
- При наступлении времени выполнения ближайшей задачи, она выполняется.
После этого снова выполняется расчёт времени до новой ближайшей задачи  
Как итог: Ардуино спит (в зависимости от периодов) 99.999% времени, просыпаясь только для проверки флага и расчёта таймера
**СМОТРИ ПРИМЕР**
---

## Старые или не актуальные либы

### TM74HC595_Gyver v1.1 [СКАЧАТЬ](https://github.com/AlexGyver/GyverLibs/releases/download/TM74HC595_Gyver/TM74HC595_Gyver.zip)
Библиотека для дисплея на сдвиговике TM74HC595 с Arduino
- Подробное описание здесь http://alexgyver.ru/tm74hc595_display/

### TM1637_Gyver v1.1
Библиотека для дисплея на сдвиговике TM1637 с Arduino
- Подробное описание здесь http://alexgyver.ru/tm1637_display/