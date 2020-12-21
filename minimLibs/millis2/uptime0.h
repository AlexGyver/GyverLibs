#ifndef uptime0_h
#define uptime0_h
#define MICROSECONDS_PER_TIMER0_OVERFLOW (clockCyclesToMicroseconds(64 * 256))
#define MILLIS_INC (MICROSECONDS_PER_TIMER0_OVERFLOW / 1000)
#define FRACT_INC ((MICROSECONDS_PER_TIMER0_OVERFLOW % 1000) >> 3)
#define FRACT_MAX (1000 >> 3)
#define MICROS_MULT (64 / clockCyclesPerMicrosecond())

void uptime0Init();
unsigned long millis0();
unsigned long micros0();
void delay0(unsigned long ms);
void delayMicroseconds0(unsigned int us);

volatile unsigned long timer0_overflow_count = 0;
volatile unsigned long timer0_millis = 0;
static unsigned char timer0_fract = 0;

void uptime0Init() {
  sei();
  TCCR0A = (1 << WGM01) | (1 << WGM00);
  TCCR0B = (1 << CS01) | (1 << CS00);
  TIMSK0 |= (1 << TOIE0);
}

ISR(TIMER0_OVF_vect){
	timer0_millis += MILLIS_INC;
	timer0_fract += FRACT_INC;
	if (timer0_fract >= FRACT_MAX) {
		timer0_fract -= FRACT_MAX;
		timer0_millis++;
	}	
	timer0_overflow_count++;
}

unsigned long millis0() {	
	uint8_t oldSREG = SREG; 			// запомнинаем были ли включены прерывания
	cli();								// выключаем прерывания
	unsigned long m = timer0_millis; 	// перехватить значение
	SREG = oldSREG; 					// если прерывания не были включены - не включаем и наоборот
	return m;						    // вернуть миллисекунды
}

unsigned long micros0() {
	uint8_t oldSREG = SREG; 						// запомнинаем были ли включены прерывания
	cli();											// выключаем прерывания
	unsigned long m = timer0_overflow_count; 		// счет переполнений
	uint8_t t = TCNT0; 								// считать содержимое счетного регистра
	if ((TIFR0 & _BV(TOV0)) && (t < 255)) 			// инкремент по переполнению
	m++;
	SREG = oldSREG; 								// если прерывания не были включены - не включаем и наоборот
	return (long)(((m << 8) + t) * MICROS_MULT);  	// вернуть микросекунды
}

void delay0(unsigned long ms) {
	uint32_t start = micros(); 
	while (ms > 0) { // ведем отсчет
		while ( ms > 0 && (micros() - start) >= 1000) {
			ms--;
			start += 1000;
		}
	}
}

void delayMicroseconds0(unsigned int us) {
#if F_CPU >= 24000000L
  us *= 6; // x6 us, = 7 cycles
  us -= 5; //=2 cycles

#elif F_CPU >= 20000000L
  __asm__ __volatile__ (
    "nop" "\n\t"
    "nop" "\n\t"
    "nop" "\n\t"
    "nop"); //just waiting 4 cycles
  if (us <= 1) return; //  = 3 cycles, (4 when true)
  us = (us << 2) + us; // x5 us, = 7 cycles
  us -= 7; // 2 cycles

#elif F_CPU >= 16000000L
  if (us <= 1) return; //  = 3 cycles, (4 when true)
  us <<= 2; // x4 us, = 4 cycles
  us -= 5; // = 2 cycles,

#elif F_CPU >= 12000000L
  if (us <= 1) return; //  = 3 cycles, (4 when true)
  us = (us << 1) + us; // x3 us, = 5 cycles
  us -= 5; //2 cycles

#elif F_CPU >= 8000000L
  if (us <= 2) return; //  = 3 cycles, (4 when true)
  us <<= 1; //x2 us, = 2 cycles
  us -= 4; // = 2 cycles

#else
  if (us <= 16) return; //= 3 cycles, (4 when true)
  if (us <= 25) return; //= 3 cycles, (4 when true), (must be at least 25 if we want to substract 22)
  us -= 22; // = 2 cycles
  us >>= 2; // us div 4, = 4 cycles

#endif
  __asm__ __volatile__ (
    "1: sbiw %0,1" "\n\t" // 2 cycles
    "brne 1b" : "=w" (us) : "0" (us) // 2 cycles
  );
}
#endif