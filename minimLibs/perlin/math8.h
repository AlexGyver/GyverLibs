#ifndef FASTLED_VERSION		// запрещаем одновременный инклуд с фастледом

#pragma once
#include <Arduino.h>

#define LIB8STATIC __attribute__ ((unused)) static inline
#define LIB8STATIC_ALWAYS_INLINE __attribute__ ((always_inline)) static inline

#if defined(__AVR_ATmega32U2__) || defined(__AVR_ATmega16U2__) || defined(__AVR_ATmega8U2__) || defined(__AVR_AT90USB162__) || defined(__AVR_ATtiny24__) || defined(__AVR_ATtiny44__) || defined(__AVR_ATtiny84__) || defined(__AVR_ATtiny25__) || defined(__AVR_ATtiny45__) || defined(__AVR_ATtiny85__) || defined(__AVR_ATtiny167__) || defined(__AVR_ATtiny87__) || defined(__AVR_ATtinyX41__) || defined(__AVR_ATtiny841__) || defined(__AVR_ATtiny441__)
#define LIB8_ATTINY 1
#endif

typedef uint8_t   fract8;
typedef uint16_t  fract16;

LIB8STATIC_ALWAYS_INLINE int8_t avg7( int8_t i, int8_t j) {
	asm volatile(
	"asr %1        \n\t"
	"asr %0        \n\t"
	"adc %0, %1    \n\t"
	: "+a" (i)
	: "a"  (j) );
	return i;
}

LIB8STATIC uint16_t scale16( uint16_t i, fract16 scale ) {
	uint32_t result;
	asm volatile(
	"  mul %A[i], %A[scale]                 \n\t"
	"  movw %A[result], r0                   \n\t"
	: [result] "=r" (result)
	: [i] "r" (i),
	[scale] "r" (scale)
	: "r0", "r1"
	);
	
	asm volatile(
	"  mul %B[i], %B[scale]                 \n\t"
	"  movw %C[result], r0                   \n\t"
	: [result] "+r" (result)
	: [i] "r" (i),
	[scale] "r" (scale)
	: "r0", "r1"
	);
	
	const uint8_t  zero = 0;
	asm volatile(
	// result.B-D += i.B x scale.A
	"  mul %B[i], %A[scale]                 \n\t"
	
	"  add %B[result], r0                   \n\t"
	"  adc %C[result], r1                   \n\t"
	"  adc %D[result], %[zero]              \n\t"
	
	// result.B-D += i.A x scale.B
	"  mul %A[i], %B[scale]                 \n\t"
	
	"  add %B[result], r0                   \n\t"
	"  adc %C[result], r1                   \n\t"
	"  adc %D[result], %[zero]              \n\t"
	
	// cleanup r1
	"  clr r1                               \n\t"
	
	: [result] "+r" (result)
	: [i] "r" (i),
	[scale] "r" (scale),
	[zero] "r" (zero)
	: "r0", "r1"
	);

	asm volatile(
	// result.A-D += i.A-B
	"  add %A[result], %A[i]                \n\t"
	"  adc %B[result], %B[i]                \n\t"
	"  adc %C[result], %[zero]              \n\t"
	"  adc %D[result], %[zero]              \n\t"
	: [result] "+r" (result)
	: [i] "r" (i),
	[zero] "r" (zero)
	);
	
	result = result >> 16;
	return result;
}

LIB8STATIC_ALWAYS_INLINE uint8_t scale8( uint8_t i, fract8 scale) {
#if defined(LIB8_ATTINY)
	uint8_t work=i;
	uint8_t cnt=0x80;
	asm volatile(
	"  inc %[scale]                 \n\t"
	"  breq DONE_%=                 \n\t"
	"  clr %[work]                  \n\t"
	"LOOP_%=:                       \n\t"	
	"  sbrc %[scale], 0             \n\t"
	"  add %[work], %[i]            \n\t"
	"  ror %[work]                  \n\t"
	"  lsr %[scale]                 \n\t"
	"  lsr %[cnt]                   \n\t"
	"brcc LOOP_%=                   \n\t"
	"DONE_%=:                       \n\t"
	: [work] "+r" (work), [cnt] "+r" (cnt)
	: [scale] "r" (scale), [i] "r" (i)
	:
	);
	return work;
#else
	asm volatile(
	// Multiply 8-bit i * 8-bit scale, giving 16-bit r1,r0
	"mul %0, %1          \n\t"
	// Add i to r0, possibly setting the carry flag
	"add r0, %0         \n\t"
	// load the immediate 0 into i (note, this does _not_ touch any flags)
	"ldi %0, 0x00       \n\t"
	// walk and chew gum at the same time
	"adc %0, r1          \n\t"
	"clr __zero_reg__    \n\t"
	: "+a" (i)      /* writes to i */
	: "a"  (scale)  /* uses scale */
	: "r0", "r1"    /* clobbers r0, r1 */ );

	/* Return the result */
	return i;
#endif
}

LIB8STATIC_ALWAYS_INLINE uint8_t qadd8( uint8_t i, uint8_t j) {
	asm volatile(
	"add %0, %1    \n\t"
	"brcc L_%=     \n\t"
	"ldi %0, 0xFF  \n\t"
	"L_%=: "
	: "+a" (i)
	: "a"  (j) );
	return i;
}

LIB8STATIC uint8_t ease8InOutQuad(uint8_t val) {
	uint8_t j=val;
	asm volatile (
	"sbrc %[val], 7 \n"
	"com %[j]       \n"
	"mul %[j], %[j] \n"
	"add r0, %[j]   \n"
	"ldi %[j], 0    \n"
	"adc %[j], r1   \n"
	"lsl r0         \n" // carry = high bit of low byte of mul product
	"rol %[j]       \n" // j = (j * 2) + carry // preserve add'l bit of precision
	"sbrc %[val], 7 \n"
	"com %[j]       \n"
	"clr __zero_reg__   \n"
	: [j] "+&a" (j)
	: [val] "a" (val)
	: "r0", "r1"
	);
	return j;
}

LIB8STATIC int16_t lerp15by16( int16_t a, int16_t b, fract16 frac)
{
	int16_t result;
	if( b > a) {
		uint16_t delta = b - a;
		uint16_t scaled = scale16( delta, frac);
		result = a + scaled;
	} else {
		uint16_t delta = a - b;
		uint16_t scaled = scale16( delta, frac);
		result = a - scaled;
	}
	return result;
}

LIB8STATIC uint16_t ease16InOutQuad( uint16_t i)
{
	uint16_t j = i;
	if( j & 0x8000 ) {
		j = 65535 - j;
	}
	uint16_t jj  = scale16( j, j);
	uint16_t jj2 = jj << 1;
	if( i & 0x8000 ) {
		jj2 = 65535 - jj2;
	}
	return jj2;
}
#endif