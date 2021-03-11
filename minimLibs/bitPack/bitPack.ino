#include "bitPack.h"

// указываем количество флагов
BitPack<10> flags;

void setup() {
  Serial.begin(9600);
  flags.clearAll(); // опустить все
  flags.set(1);   // поднять флаг
  flags.set(3);
  flags.write(3, 1);
  Serial.println(flags.read(0));  // прочитать флаг
  Serial.println(flags.read(1));
  Serial.println(flags.read(2));
  flags[3] = 0;				// можно писать через []
  Serial.println(flags[3]); // можно читать через []
}

void loop() {
  // put your main code here, to run repeatedly:

}
