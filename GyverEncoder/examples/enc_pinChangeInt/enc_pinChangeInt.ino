// пример с прерываниями pinChangeInterrupt (прерывания на любом пине)
// только для ATmega328 (UNO, Nano, Pro Mini)

#define SW 0
#define DT 2
#define CLK 3

#include "GyverEncoder.h"
Encoder enc1(CLK, DT, SW);

void setup() {
  Serial.begin(9600);

  // настроить PCINT
  attachPCINT(CLK);
  attachPCINT(DT);
}

void loop() {
  enc1.tick();    // оставляем тут для работы "временных" функций и антидребезга
  
  if (enc1.isRight()) Serial.println("Right");         // если был поворот
  if (enc1.isLeft()) Serial.println("Left");

  if (enc1.isRightH()) Serial.println("Right holded"); // если было удержание + поворот
  if (enc1.isLeftH()) Serial.println("Left holded");
}

// функция для настройки PCINT для ATmega328 (UNO, Nano, Pro Mini)
uint8_t attachPCINT(uint8_t pin) {
  if (pin < 8) { // D0-D7 // PCINT2
    PCICR |= (1 << PCIE2);
    PCMSK2 |= (1 << pin);
    return 2;
  }
  else if (pin > 13) { //A0-A5  // PCINT1
    PCICR |= (1 << PCIE1);
    PCMSK1 |= (1 << pin - 14);
    return 1;
  }
  else { // D8-D13  // PCINT0
    PCICR |= (1 << PCIE0);
    PCMSK0 |= (1 << pin - 8);
    return 0;
  }
}

// Векторы PCINT, нужно кинуть сюда тики
// не обязательно в каждый вектор, достаточно в тот, который задействован
// пины 0-7: PCINT2
// пины 8-13: PCINT0
// пины A0-A5: PCINT1
ISR(PCINT0_vect) {
  //enc1.tick();
}
ISR(PCINT1_vect) {
  //enc1.tick();
}
ISR(PCINT2_vect) {
  enc1.tick();
}
