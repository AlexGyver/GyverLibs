#ifndef GyverRC_h
#define GyverRC_h
#include <Arduino.h>

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
	
	int getNorm();
	int getHold();
	
	boolean isTurn();	
	
	boolean isRight();
	boolean isLeft();	
	boolean isRightH();
	boolean isLeftH();
	
	boolean isPress();
	boolean isRelease();
    boolean isHolded();
	boolean isHold();
	
  private:
    byte _CLK, _DT, _SW;
	
    boolean DT_now, DT_last, SW_state, hold_flag, butt_flag, turn_flag;
	boolean isRight_f, isLeft_f, isRightH_f, isLeftH_f, isTurn_f;
	boolean isPress_f, isRelease_f, isHolded_f, isHold_f;
	
	int _norm, _hold;
	int _normMin, _normMax, _holdMin, _holdMax;
	uint8_t _norm_step, _hold_step;
	
	unsigned long debounce_timer;
};
 
#endif