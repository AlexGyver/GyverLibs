#include <directTimers.h>
/* пример генерации стандартного шим 10 бит  */

void setup() {
  TIMER1_setClock(PRESCALER_1); // настройка делителя таймера
  TIMER1_setMode(FAST_PWM_10BIT); // настройка режима работы таймера
  TIMER1_COMPA_mode(PWM); // настроили аппаратные выходы с таймера в режим шим
  TIMER1_COMPB_mode(PWM);

  pinMode(9, OUTPUT); // аппаратный выход с таймера А
  pinMode(10, OUTPUT); // аппаратный выход с таймера B
}

void loop() {
  TIMER1_COMPA_setValue(analogRead(A0)); // заполнение шим канала А (pin 9) регулируется потенциометром
  TIMER1_COMPB_setValue(analogRead(A1)); // аналогично с каналом B (pin 10)
}
