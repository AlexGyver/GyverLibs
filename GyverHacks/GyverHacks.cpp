#include "GyverHacks.h"
#include <Arduino.h>
#define STOPPER 0  

// ***************************** GTimer *****************************
GTimer::GTimer() {}

GTimer::GTimer(uint16_t interval) {
	_interval = interval;
	_timer = millis() + _interval;
}

void GTimer::setInterval(uint16_t interval) {
	_interval = interval;
}

boolean GTimer::isReady() {
	if ((long)millis() > _timer) {
		_timer = millis() + _interval;
		return true;
	} else {
		return false;
	}
}

void GTimer::reset() {
	_timer = millis() + _interval;
}

// ***************************** GFilterRA *****************************
GFilterRA::GFilterRA() {}
GFilterRA::GFilterRA(float coef, uint16_t interval) {
	_coef = coef;
	_filterInterval = interval;
}

void GFilterRA::setCoef(float coef) {
	_coef = coef;
}
void GFilterRA::setStep(uint16_t interval) {
	_filterInterval = interval;
}

float GFilterRA::filteredTime(int16_t value) {
	if (millis() > _filterTimer) {
		_filterTimer = millis() + _filterInterval;
		_lastValue = _lastValue * ((float)1 - _coef) + (float)value * _coef;
	}
	return _lastValue;
}

float GFilterRA::filtered(int16_t value) {
	_lastValue = _lastValue * ((float)1 - _coef) + (float)value * _coef;
	return _lastValue;
}

// ***************************** GMedian3 *****************************
GMedian3::GMedian3() {}

uint16_t GMedian3::filtered(uint16_t value) {
	buffer[counter] = value;
	if (++counter > 2) counter = 0;
	
	int a = buffer[0]; 
	int b = buffer[1];
	int c = buffer[2];
	int middle;	
	
  if ((a <= b) && (a <= c)) {
    middle = (b <= c) ? b : c;
  }
  else {
    if ((b <= a) && (b <= c)) {
      middle = (a <= c) ? a : c;
    }
    else {
      middle = (a <= b) ? a : b;
    }
  }
  return middle;
}

// ***************************** GMedian *****************************
GMedian::GMedian() {}
uint16_t GMedian::filtered(uint16_t value) {
	struct pair
 {
   struct pair   *point;                              /* Pointers forming list linked in sorted order */
   uint16_t  value;                                   /* Values to sort */
 };
 static struct pair buffer[MEDIAN_FILTER_SIZE] = {0}; /* Buffer of nwidth pairs */
 static struct pair *datpoint = buffer;               /* Pointer into circular buffer of data */
 static struct pair small = {NULL, STOPPER};          /* Chain stopper */
 static struct pair big = {&small, 0};                /* Pointer to head (largest) of linked list.*/

 struct pair *successor;                              /* Pointer to successor of replaced data item */
 struct pair *scan;                                   /* Pointer used to scan down the sorted list */
 struct pair *scanold;                                /* Previous value of scan */
 struct pair *median;                                 /* Pointer to median */
 uint16_t i;

 if (value == STOPPER)
 {
   value = STOPPER + 1;                             /* No stoppers allowed. */
 }

 if ( (++datpoint - buffer) >= MEDIAN_FILTER_SIZE)
 {
   datpoint = buffer;                               /* Increment and wrap data in pointer.*/
 }

 datpoint->value = value;                           /* Copy in new value */
 successor = datpoint->point;                       /* Save pointer to old value's successor */
 median = &big;                                     /* Median initially to first in chain */
 scanold = NULL;                                    /* Scanold initially null. */
 scan = &big;                                       /* Points to pointer to first (largest) value in chain */

 /* Handle chain-out of first item in chain as special case */
 if (scan->point == datpoint)
 {
   scan->point = successor;
 }
 scanold = scan;                                     /* Save this pointer and   */
 scan = scan->point ;                                /* step down chain */

 /* Loop through the chain, normal loop exit via break. */
 for (i = 0 ; i < MEDIAN_FILTER_SIZE; ++i)
 {
   /* Handle odd-numbered item in chain  */
   if (scan->point == datpoint)
   {
     scan->point = successor;                      /* Chain out the old value.*/
   }

   if (scan->value < value)                        /* If value is larger than scanned value,*/
   {
     datpoint->point = scanold->point;             /* Chain it in here.  */
     scanold->point = datpoint;                    /* Mark it chained in. */
     value = STOPPER;
   };

   /* Step median pointer down chain after doing odd-numbered element */
   median = median->point;                       /* Step median pointer.  */
   if (scan == &small)
   {
     break;                                      /* Break at end of chain  */
   }
   scanold = scan;                               /* Save this pointer and   */
   scan = scan->point;                           /* step down chain */

   /* Handle even-numbered item in chain.  */
   if (scan->point == datpoint)
   {
     scan->point = successor;
   }

   if (scan->value < value)
   {
     datpoint->point = scanold->point;
     scanold->point = datpoint;
     value = STOPPER;
   }

   if (scan == &small)
   {
     break;
   }

   scanold = scan;
   scan = scan->point;
 }
 return median->value;
}

// ***************************** GABfilter *****************************
GABfilter::GABfilter(float delta, float sigma_process, float sigma_noise) {
	dt = delta;
    float lambda = (float)sigma_process * dt * dt / sigma_noise;
    float r = (4 + lambda - (float)sqrt(8 * lambda + lambda * lambda)) / 4;
    a = (float)1 - r * r;
    b = (float)2 * (2 - a) - 4 * (float)sqrt(1 - a);
}
void GABfilter::setParameters(float delta, float sigma_process, float sigma_noise) {
	dt = delta;
    float lambda = (float)sigma_process * dt * dt / sigma_noise;
    float r = (4 + lambda - (float)sqrt(8 * lambda + lambda * lambda)) / 4;
    a = (float)1 - r * r;
    b = (float)2 * (2 - a) - 4 * (float)sqrt(1 - a);
}
uint16_t GABfilter::filtered(uint16_t value) {
  xm = value;
  xk = xk_1 + ((float) vk_1 * dt );
  vk = vk_1;
  rk = xm - xk;
  xk += (float)a * rk;
  vk += (float)( b * rk ) / dt;
  xk_1 = xk;
  vk_1 = vk;
  return xk_1;
}
/*
float GABfilter::filtered(float value) {
  xm = value;
  xk = xk_1 + ((float) vk_1 * dt );
  vk = vk_1;
  rk = xm - xk;
  xk += (float)a * rk;
  vk += (float)( b * rk ) / dt;
  xk_1 = xk;
  vk_1 = vk;
  return xk_1;
}*/

// ***************************** GKalman *****************************
#include <math.h>
GKalman::GKalman(float mea_e, float est_e, float q)
{
  GKalman::setParameters(mea_e, est_e, q);
}

void GKalman::setParameters(float mea_e, float est_e, float q)
{
  _err_measure=mea_e;
  _err_estimate=est_e;
  _q = q;
}

float GKalman::filtered(float value)
{
  _kalman_gain = _err_estimate/(_err_estimate + _err_measure);
  _current_estimate = _last_estimate + _kalman_gain * (value - _last_estimate);
  _err_estimate =  (1.0 - _kalman_gain)*_err_estimate + fabs(_last_estimate-_current_estimate)*_q;
  _last_estimate=_current_estimate;

  return _current_estimate;
}

// ***************************** Others *****************************
#if defined(__AVR_ATmega328__) || defined(__AVR_ATmega328P__) || defined(__AVR_ATmega168__) || defined(__AVR_ATmega88__)
int vcc_const = 1100;

int getVCC() {
  //reads internal 1V1 reference against VCC
  #if defined(__AVR_ATtiny84__) || defined(__AVR_ATtiny44__)
    ADMUX = _BV(MUX5) | _BV(MUX0); // For ATtiny84
  #elif defined(__AVR_ATtiny85__) || defined(__AVR_ATtiny45__)
    ADMUX = _BV(MUX3) | _BV(MUX2); // For ATtiny85/45
  #elif defined(__AVR_ATmega1284P__)
    ADMUX = _BV(REFS0) | _BV(MUX4) | _BV(MUX3) | _BV(MUX2) | _BV(MUX1);  // For ATmega1284
  #else
    ADMUX = _BV(REFS0) | _BV(MUX3) | _BV(MUX2) | _BV(MUX1);  // For ATmega328
  #endif
  delay(2); // Wait for Vref to settle
  ADCSRA |= _BV(ADSC); // Convert
  while (bit_is_set(ADCSRA, ADSC));
  uint8_t low = ADCL;
  unsigned int val = (ADCH << 8) | low;
  //discard previous result
  ADCSRA |= _BV(ADSC); // Convert
  while (bit_is_set(ADCSRA, ADSC));
  low = ADCL;
  val = (ADCH << 8) | low;
  
  return ((long)1024 * vcc_const) / val;  
}

void setConstant(uint16_t voltage) {
	vcc_const = (long)1100 * voltage / getVCC();
}

int getVoltage(uint8_t pin) {
	return (long)(getVCC() * (float)analogRead(pin) / 1024);
}

float getTemp() {
  /* from the data sheet
    Temperature / °C -45°C +25°C +85°C
    Voltage     / mV 242 mV 314 mV 380 mV
  */
  unsigned int wADC;
  double t;
  // The internal temperature has to be used
  // with the internal reference of 1.1V.
  // Channel 8 can not be selected with
  // the analogRead function yet.
  // Set the internal reference and mux.
  ADMUX = (_BV(REFS1) | _BV(REFS0) | _BV(MUX3));
  ADCSRA |= _BV(ADEN);  // enable the ADC
  delay(10);            // wait for voltages to become stable.
  ADCSRA |= _BV(ADSC);  // Start the ADC
  // Detect end-of-conversion
  while (bit_is_set(ADCSRA, ADSC));
  // Reading register "ADCW" takes care of how to read ADCL and ADCH.
  wADC = ADCW;
  // The offset of 324.31 could be wrong. It is just an indication.
  t = (wADC - 324.31 ) / 1.22;
  // The returned temperature is in degrees Celsius.
  return (t);
}

void setPWM10bit(uint8_t mode) {
	TCCR1A = TCCR1A & 0xe0 | 3;		// ставим 10 бит
	switch (mode) {
		case 0: TCCR1B = TCCR1B & 0xe0 | 0x0d; break;
		case 1: TCCR1B = TCCR1B & 0xe0 | 0x0c; break;
		case 2: TCCR1B = TCCR1B & 0xe0 | 0x0b; break;
		case 3: TCCR1B = TCCR1B & 0xe0 | 0x0a; break;
		case 4: TCCR1B = TCCR1B & 0xe0 | 0x09; break;
	}
}

void setPWMPrescaler(uint8_t pin, uint16_t prescale) {
  byte mode;
  if (pin == 5 || pin == 6 || pin == 9 || pin == 10) {
    switch (prescale) {
      case    1: mode = 0b001; break;
      case    8: mode = 0b010; break;
      case   64: mode = 0b011; break;
      case  256: mode = 0b100; break;
      case 1024: mode = 0b101; break;
      default: return;
    }
  } else if (pin == 3 || pin == 11) {
    switch (prescale) {
      case    1: mode = 0b001; break;
      case    8: mode = 0b010; break;
      case   32: mode = 0b011; break;
      case   64: mode = 0b100; break;
      case  128: mode = 0b101; break;
      case  256: mode = 0b110; break;
      case 1024: mode = 0b111; break;
      default: return;
    }
  }
  if (pin == 5 || pin == 6) {
    TCCR0B = TCCR0B & 0b11111000 | mode;
  } else if (pin == 9 || pin == 10) {
    TCCR1B = TCCR1B & 0b11111000 | mode;
  } else if (pin == 3 || pin == 11) {
    TCCR2B = TCCR2B & 0b11111000 | mode;
  }
}

void fastAnalogWrite(uint8_t pin, uint8_t duty) {
	if (duty == 0) digitalWrite(pin, LOW);
	else if (duty == 255) digitalWrite(pin, HIGH);
	else 
	{
		switch (pin) {
		case 5:
		sbi(TCCR0A, COM0B1);
		OCR0B = duty;
		break;
		case 6:
		sbi(TCCR0A, COM0A1);
		OCR0A = duty;
		break;
		case 10:
		sbi(TCCR1A, COM1B1);
		OCR1B = duty;
		break;
		case 9:
		sbi(TCCR1A, COM1A1);
		OCR1A = duty;
		break;
		case 3:
		sbi(TCCR2A, COM2B1);
		OCR2B = duty;
		break;
		case 11:
		sbi(TCCR2A, COM2A1);
		OCR2A = duty;
		break;
		default:
		break;
		}
	}
}
#endif