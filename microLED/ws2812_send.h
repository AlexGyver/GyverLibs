#pragma once

// =================== ВЫВОД ===================
#define NOP1 "nop \n\t" 
#define NOP2 "rjmp .+0	\n\t"

#if F_CPU == 8000000
#define _DELAY_ 
#define MAX_DATA_SPEED
#elif F_CPU == 12000000
#define _DELAY_   
#elif F_CPU == 16000000
#define _DELAY_ NOP2             
#elif F_CPU == 20000000 
#define _DELAY_ NOP2 NOP2 NOP1
#endif

#if defined(ORDER_GRB)
#define ORDER0	1
#define ORDER1	0
#define ORDER2	2
#elif defined(ORDER_RGB)
#define ORDER0	0
#define ORDER1	1
#define ORDER2	2
#elif defined(ORDER_BRG)
#define ORDER0	2
#define ORDER1	0
#define ORDER2	1
#else
#define ORDER0	1
#define ORDER1	0
#define ORDER2	2
#endif

#if (COLOR_DEBTH == 2)
#define PTR_TYPE uint16_t*
#else
#define PTR_TYPE uint8_t*
#endif


void WS2812B_sendData (PTR_TYPE data, uint16_t datlen, uint8_t maskhi, uint8_t *port, uint8_t *portreg, uint8_t bright) {
	uint8_t ctr,masklo;
	uint8_t sreg_prev;
	uint8_t loopData[3];
	uint8_t *data_ptr = loopData;	

	masklo = ~maskhi & *port;
	maskhi |= *port;
	sreg_prev=SREG;
	cli();  

	while (datlen) {
		data_ptr = loopData;
#if (COLOR_DEBTH == 1)
		// 8 бит
		loopData[ORDER0] = (*data & 0b11000000) >> bright;
		loopData[ORDER1] = ((*data & 0b00111000) << 2) >> bright;
		loopData[ORDER2] = ((*data & 0b00000111) << 5) >> bright;
		*data++;
		datlen--;
#elif (COLOR_DEBTH == 2)
		// 16 бит
		loopData[ORDER0] = (((*data & 0b1111100000000000) >> 8) * bright) >> 8;
		loopData[ORDER1] = (((*data & 0b0000011111100000) >> 3) * bright) >> 8;
		loopData[ORDER2] = (((*data & 0b0000000000011111) << 3) * bright) >> 8;
		
		*data++;
		datlen -= 2;
#elif (COLOR_DEBTH == 3)
		// 32 бит		
		loopData[ORDER0] = (*(data++) * bright) >> 8;
		loopData[ORDER1] = (*(data++) * bright) >> 8;
		loopData[ORDER2] = (*(data++) * bright) >> 8;
		datlen -= 3;
#endif
		int8_t sendCounter = 3;
		while(sendCounter--) {
			asm volatile(
			// [clocks] - (action)
			"ldi   %[counter] ,8  \n\t"     // закинуть 8 в счетчик циклов 
			
			"loop:           	  \n\t"     // начало цикла отправки
			"st X, %[set_hi]      \n\t"     // установить high на пине 
			"sbrs	%[data], 7    \n\t"   	// если нужно отправить '1' , пропустить след команду 
			"st	X, %[set_lo] 	  \n\t"   	// сбросить пин в low
			"lsl  %[data]    	  \n\t"     // сдвинуть данные влево на 1
			"dec   %[counter]     \n\t"     // отнять от счетчика 1 
			#ifndef MAX_DATA_SPEED				// выключение доп задержки
			"rjmp .+0	          \n\t"	    // доп задержка из 2х тактов
			"rjmp .+0	          \n\t"	    // доп задержка из 2х тактов
			"rjmp .+0	          \n\t"	    // доп задержка из 2х тактов
			"rjmp .+0	          \n\t"	    // доп задержка из 2х тактов
			"rjmp .+0	          \n\t"	    // доп задержка из 2х тактов
			#endif
			"brcc to_end 		  \n\t"  	// перейти если уже вывели low
			_DELAY_		                    // основная задержка
			"st  X,%[set_lo]      \n\t"     // сбросить пин в low
			"to_end:              \n\t"     // точка куда переходим при пропуске				
			"brne  loop		      \n\t"     // вернуться в начало цикла
			
			:	[counter] "=&d" (ctr)
			:	[data] "r" (*data_ptr++), "x" (port), [set_hi] "r" (maskhi), [set_lo] "r" (masklo)
			);
		}
	}

	SREG=sreg_prev;
}