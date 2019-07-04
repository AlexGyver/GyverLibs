#include <GyverPWM.h>

void setup() {
  pinMode(3, OUTPUT);
  pinMode(5, OUTPUT);
  pinMode(9, OUTPUT);
  pinMode(10, OUTPUT);
  
  // Данные функции убирают один ШИМ выход у 8-ми битных таймеров, оставляя нам ШИМ пины D3, D5, D9 и D10 на ATmega328

  // запустить ШИМ на D3 с частотой 66.666 Гц, режим FAST_PWM
  PWM_frequency(3, 66666, FAST_PWM);

  // запустить ШИМ на D5 с частотой 25'000 Гц, режим CORRECT_PWM. Сломает millis()!
  PWM_frequency(5, 25000, CORRECT_PWM);

  // запустить ШИМ на D9 с частотой 150'000 Гц, режим FAST_PWM
  PWM_frequency(9, 150000, FAST_PWM);

  // запустить ШИМ на D10 с частотой 150'000 Гц, режим FAST_PWM
  PWM_frequency(10, 150000, FAST_PWM);
  // пины 9 и 10 работают на одной частоте, запустить на разных не получится!
}

void loop() {
  PWM_set(3, analogRead(0) / 4);  // заполнение регулируем потенциометром на А0 (8 бит)
  PWM_set(5, analogRead(0) / 4);  // заполнение регулируем потенциометром на А0 (8 бит)

  PWM_set(9, analogRead(0) / 4);  // заполнение регулируем потенциометром на А0 (8 бит)
  PWM_set(10, 255 - analogRead(0) / 4);  // заполнение регулируем потенциометром на А0 (8 бит)
}
