/***********************************************************************************
* Developed for AlexGyver https://github.com/AlexGyver  by Egor 'Nich1con' Zaharov *
* Support MCU's : AVR ATmega328p/32U4/2560 & ATtiny85/84/167 					   *												  
* Distributed under a free license indicating the source						   * 														  
* Version 2.0 from 03.11.19														   *
* Version 2.1 from 23.03.20 - внёс реализацию в класс							   *
***********************************************************************************/ 

#pragma once
#include <Arduino.h>

#if defined(__AVR_ATtiny85__) || defined(__AVR_ATtiny167__)
#define WDTCSR WDTCR
#endif

#define	WATCHDOG WDT_vect

/**************************
* Watchdog operating mode *
**************************/
#define RESET_MODE				0x08	// To reset the CPU if there is a timeout
#define INTERRUPT_MODE			0x40	// Timeout will cause an interrupt
#define INTERRUPT_RESET_MODE	0x48	// First time-out interrupt , the second time out - reset


/**********************************
* Watchdog time-out vs prescaler: *
**********************************/
#define WDT_PRESCALER_2		0x00	// (16 ± 1.6) ms
#define WDT_PRESCALER_4		0x01	// (32 ± 3.2) ms
#define WDT_PRESCALER_8		0x02	// (64 ± 6.4) ms
#define WDT_PRESCALER_16	0x03	// (128 ± 12.8) ms
#define WDT_PRESCALER_32	0x04	// (256 ± 25.6) ms
#define WDT_PRESCALER_64	0x05	// (512 ± 51.2) ms
#define WDT_PRESCALER_128	0x06	// (1024 ± 102.4) ms
#define WDT_PRESCALER_256	0x07	// (2048 ± 204.8) ms
#define WDT_PRESCALER_512	0x08	// (4096 ± 409.6) ms
#define WDT_PRESCALER_1024	0x09	// (8192 ± 819.2) ms

class GyverWDT {

public:
	// сброс
	void reset() {                              
		asm volatile ("WDR");
	}
	
	// отключить WDT
	void disable() {                            
		uint8_t sregCopy = SREG;				// Save global interrupt settings
		cli();									// Disable global interrupts
		WDTCSR = ((1 << WDCE) | (1 << WDE));	// Allow changing settings
		WDTCSR = 0x00;							// Reset all watchdog settings
		SREG = sregCopy;						// Restore global interrupt settings
	}
	
	// включить WDT с настройками:
	// mode:
	// RESET_MODE - сброс при зависании (при тайм-ауте WDT)
	// INTERRUPT_MODE - прерывание при зависании (при тайм-ауте WDT)
	// INTERRUPT_RESET_MODE - первый таймаут - прерывание, второй - сброс
	// prescaler:
	// WDT_PRESCALER_2, WDT_PRESCALER_4... WDT_PRESCALER_1024
	void enable(uint8_t mode, uint8_t prescaler) {  
		uint8_t sregCopy , wdtRegister;													// Local variables of a function
		wdtRegister = mode | ((prescaler > 7) ? 0x20 | (prescaler - 8) : prescaler);	// Creating new watchdog settings
		sregCopy = SREG;																// Save global interrupt settings
		cli();																			// Disable global interrupts
		WDTCSR = ((1 << WDCE) | (1 << WDE));											// Allow changing settings
		WDTCSR = wdtRegister;															// Setting new watchdog settings
		SREG = sregCopy;																// Restore global interrupt settings
	}	
private:
};

extern GyverWDT Watchdog;