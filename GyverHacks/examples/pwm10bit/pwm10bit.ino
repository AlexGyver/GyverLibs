#include "GyverHacks.h"

void setup() {
  setPWM10bit(KHZ_16);		// установить частоту ШИМ на 9 и 10 пине для 10 бит
  analogWrite(9, 666);	  // запустить ШИМ на 9 пине
}

void loop() {
  // put your main code here, to run repeatedly:

}
