#include "crc8.h"

void setup() {
  Serial.begin(9600);
  byte bytes[] = {0, 1, 2, 3, 4, 5, 6, 7};

  // посчитает crc массива до предпоследнего знака
  byte crc = crc8_array(bytes, sizeof(bytes) - 1);
  Serial.println(crc);

  bytes[7] = crc;

  // выведет 0, так как последним указали предыдущий crc
  crc = crc8_array(bytes, sizeof(bytes));
  Serial.println(crc);
}

void loop() {

}
