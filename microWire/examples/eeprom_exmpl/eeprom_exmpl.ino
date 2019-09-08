#include <microI2C.h>

byte data = 228; // запишем 228
int addr = 1337; // в ячейку 1337

void setup() {
  I2C_init(); // без параметра - 400 кгц
  /* запишем байт в EEPROM AT24C32 */

  I2C_start();
  I2C_transmit(0b10101110); // 0b1010xxx + write(0) => [xxx(ноги микры) == 111] => 0b10101110;
  I2C_transmit(highByte(addr)); // кидаем адрес ячейки
  I2C_transmit(lowByte(addr));
  I2C_transmit(data); // пишем сам байт
  I2C_stop(); // окончили передачу
  delay(3); // при циклической запяси - обязатльно

  /*
    // если хотим проверить , что байт записался верно
    Serial.begin(9600);
    I2C_start();
    Serial.println(TWSR, HEX);
    I2C_transmit(0b10101110); // 0b1010111 + write(0)
    Serial.println(TWSR, HEX);
    I2C_transmit(highByte(addr)); // кидаем адрес ячейки
    I2C_transmit(lowByte(addr));
    I2C_start();
    I2C_transmit(0b10101111); // 0b1010111 + read(1)
    Serial.print("data: ");
    Serial.println(I2C_receive_nack());
    I2C_stop();
  */

  // запишем во все ячейки от 0 до 255 ( переполнение байта )
  for ( int i = 0; i < 4095; i++) {
    EE_write(0x57, i, i); // 0x57 == 0b1010111 - адрес нашего чипа без бита чтения записи (7бит)
  }
  /*
    // выплюнем все содержимое в сериал
    Serial.begin(9600);

    for ( int i = 0; i < 4095; i++) {
      Serial.println(EE_read(0x57, i));
    }
  */
}

void loop() {}

/* оформим чтение запись в виде функций */

void EE_write(uint8_t chipAddr , uint16_t cellAddr , uint8_t data) {
  I2C_start();
  I2C_transmit((chipAddr << 1)); // "write"
  I2C_transmit(highByte(cellAddr));
  I2C_transmit(lowByte(cellAddr));
  I2C_transmit(data);
  I2C_stop();
  delay(3);
}

uint8_t EE_read(uint8_t chipAddr , uint16_t cellAddr) {
  I2C_start();
  I2C_transmit((chipAddr << 1)); // "write"
  I2C_transmit(highByte(cellAddr));
  I2C_transmit(lowByte(cellAddr));
  I2C_start();
  I2C_transmit((chipAddr << 1) | 0x1 ); // "read"
  uint8_t data = I2C_receive_nack();
  I2C_stop();
  return data;
}
