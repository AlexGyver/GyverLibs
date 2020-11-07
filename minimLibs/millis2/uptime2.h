#pragma once
#define MICROSECONDS_PER_TIMER2_OVERFLOW (clockCyclesToMicroseconds(64 * 256))
#define MILLIS2_INC (MICROSECONDS_PER_TIMER2_OVERFLOW / 1000)
#define FRACT2_INC ((MICROSECONDS_PER_TIMER2_OVERFLOW % 1000) >> 3)
#define FRACT2_MAX (1000 >> 3)
#define MICROS2_MULT (64 / clockCyclesPerMicrosecond())

void uptime2Init();
unsigned long millis2();
unsigned long micros2();
void delay2(unsigned long ms);
void delayMicroseconds2(unsigned int us);

volatile unsigned long timer2_overflow_count = 0;
volatile unsigned long timer2_millis = 0;
static unsigned char timer2_fract = 0;

void uptime2Init() {
  sei();
  TCCR2A = (1 << WGM20) | (1 << WGM21);
  TCCR2B = 1 << CS22;
  TIMSK2 = 1 << TOIE2;
}

ISR(TIMER2_OVF_vect) {
  timer2_millis += MILLIS2_INC;
  timer2_fract += FRACT2_INC;
  if (timer2_fract >= FRACT2_MAX) {
    timer2_fract -= FRACT2_MAX;
    timer2_millis++;
  }
  timer2_overflow_count++;
}

unsigned long millis2() {
  uint8_t oldSREG = SREG;           // запомнинаем были ли включены прерывания
  cli();                            // выключаем прерывания
  unsigned long m = timer2_millis;  // перехватить значение
  SREG = oldSREG;                   // если прерывания не были включены - не включаем и наоборот
  return m;                         // вернуть миллисекунды
}

unsigned long micros2() {
  uint8_t oldSREG = SREG;                   // запомнинаем были ли включены прерывания
  cli();                                    // выключаем прерывания
  unsigned long m = timer2_overflow_count;  // счет переполнений
  uint8_t t = TCNT2;                        // считать содержимое счетного регистра
  if ((TIFR2 & _BV(TOV2)) && (t < 255))     // инкремент по переполнению
    m++;
  SREG = oldSREG;                           // если прерывания не были включены - не включаем и наоборот
  return (long)(((m << 8) + t) * MICROS2_MULT);  // вернуть микросекунды
}

void delay2(unsigned long ms) {
  uint32_t start = micros();
  while (ms > 0) { // ведем отсчет
    while ( ms > 0 && (micros() - start) >= 1000) {
      ms--;
      start += 1000;
    }
  }
}

void delayMicroseconds2(unsigned int us) {
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
