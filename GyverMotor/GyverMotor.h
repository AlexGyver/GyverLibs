#ifndef GyverMotor_h
#define GyverMotor_h
#include <Arduino.h>

/*
	Библиотека для удобного управления моторчиками через драйвер
*/

class GMotor
{
  public:
	GMotor(uint8_t dig_pin, uint8_t pwm_pin);
	void init();
	void init10bit(uint32_t freq);
	void setSpeed(uint8_t duty);
	void setSpeed10bit(uint16_t duty);
	void setMode(uint8_t mode);
	void reverse(boolean reverse);
  private:
	void runFrw();
	void runBkw();
	void runFrw10bit();
	void runBkw10bit();
	uint8_t _dig_pin, _pwm_pin, _mode, _reverse;
	int16_t _duty;
};

#define FORWARD		0
#define BACKWARD	1
#define STOP		2

#endif