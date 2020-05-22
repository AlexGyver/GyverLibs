#pragma once

// быстрый медианный фильтр 3-го порядка
template < typename TYPE >
class GMedian3 {
public:
	TYPE filtered(TYPE value) {	// возвращает фильтрованное значение
		buffer[_counter] = value;
		if (++_counter > 2) _counter = 0;
		
		TYPE middle;	
		
		if ((buffer[0] <= buffer[1]) && (buffer[0] <= buffer[2])) {
			middle = (buffer[1] <= buffer[2]) ? buffer[1] : buffer[2];
		}
		else {
			if ((buffer[1] <= buffer[0]) && (buffer[1] <= buffer[2])) {
				middle = (buffer[0] <= buffer[2]) ? buffer[0] : buffer[2];
			}
			else {
				middle = (buffer[0] <= buffer[1]) ? buffer[0] : buffer[1];
			}
		}
		return middle;
	}
	
private:
	TYPE buffer[3];
	uint8_t _counter = 0;
};