// пример изменения частоты ШИМ на ШИМ пинах atmega328
// при изменении ШИМ у таймера 0 функции времени начинают работать некорректно
// в библиотеке есть fixed функции, смотри пример fixedTime

#include "GyverHacks.h"

void setup() {  
  setPWMprescaler(5, 2);	// установить режим (ШИМ пин, режим) в этом случае на пин 5 частота 7,8 кГц
  /*
Timer 0 (пины 5 и 6) режим 8 bit (ВЛИЯЕТ НА РАБОТУ millis() и delay())
mode	value	частота
1		0x01	62.5 kHz
2		0x02	7.8 kHz
3		0x03	976 Hz		<-- по умолч.
4		0x04	244 Hz
5		0x05	61 Hz

Timer 1 (пины 9 и 10) режим 8 bit (ВЛИЯЕТ НА РАБОТУ servo)
mode	value	частота
1		0x09	62.5 kHz
2		0x01	31.4 kHz
3		0x0a	7.8 kHz
4		0x02	4 kHz
5		0x0b	976 Hz
6		0x03	490 Hz		<-- по умолч.
7		0x0c	244 Hz
8		0x04	122 Hz
9		0x0d	61 Hz
10		0x05	30 Hz


Timer 1 (пины 9 и 10) режим 10 bit (ВЛИЯЕТ НА РАБОТУ servo)
mode	value	частота
1		0x09	15.6 kHz
2		0x01	7.8 kHz
3		0x0a	2 kHz
4		0x02	977 Hz
5		0x0b	244 Hz
6		0x03	122 Hz		<-- по умолч.
7		0x0c	61 Hz
8		0x04	30 Hz
9		0x0d	15 Hz
10		0x05	7.6 Hz


Timer 2 (пины 3 и 11) режим 8 bit (ВЛИЯЕТ НА РАБОТУ tone())
mode	value	частота
1		0x01	31.4 kHz
2		0x02	4 kHz
3		0x03	980 Hz
4		0x04	490 Hz		<-- по умолч.
5		0x05	245 Hz
6		0x06	122 Hz
7		0x07	30 Hz
*/
  
  pinMode(5, OUTPUT);
  setPWM(5, 125);			    // запустить ШИМ
}

void loop() {

}
