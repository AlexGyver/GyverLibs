#pragma once

void crc8_update(uint8_t &crc, uint8_t data) {
#if defined (__AVR__)
	// резкий алгоритм для AVR
	uint8_t counter;
	uint8_t buffer;
	asm volatile (
	"EOR %[crc_out], %[data_in] \n\t"
	"LDI %[counter], 8          \n\t"
	"LDI %[buffer], 0x8C        \n\t"
	"_loop_start_%=:            \n\t"
	"LSR %[crc_out]             \n\t"
	"BRCC _loop_end_%=          \n\t"
	"EOR %[crc_out], %[buffer]  \n\t"
	"_loop_end_%=:              \n\t"
	"DEC %[counter]             \n\t"
	"BRNE _loop_start_%="
	: [crc_out]"=r" (crc), [counter]"=d" (counter), [buffer]"=d" (buffer)
	: [crc_in]"0" (crc), [data_in]"r" (data)
	);
#else
	// обычный для всех остальных
	uint8_t i = 8;
	while (i--) {
		crc = ((crc ^ data) & 1) ? (crc >> 1) ^ 0x8C : (crc >> 1);
		data >>= 1;
	}
#endif
}

byte crc8_array(byte *buffer, byte size) {
	byte crc = 0;
	for (byte i = 0; i < size; i++) crc8_update(crc, buffer[i]);
	return crc;
}
