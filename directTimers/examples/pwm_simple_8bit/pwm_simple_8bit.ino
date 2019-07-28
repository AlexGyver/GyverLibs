#include <directTimers.h>
/* пример генерации быстрого шим 8 бит с частотой 31 кгц и коррекцией фазы */
void setup() {
  TIMER2_setClock(PRESCALER_1); // настройка делителя таймера
  TIMER2_setMode(PHASECORRECT_PWM_8BIT); // настройка режима работы таймера
  TIMER2_COMPA_mode(PWM); // прямой шим канал А
  TIMER2_COMPA_mode(INVERT_PWM); // инверсный шим канал B

  pinMode(11, OUTPUT); // аппаратный выход с таймера А
  pinMode(3, OUTPUT); // аппаратный выход с таймера B
}

void loop() {
  int duty = analogRead(A0) / 4;
  TIMER1_COMPA_setValue(duty); // заполнение шим канала А (pin 11) регулируется потенциометром
  TIMER1_COMPB_setValue(duty); // заполнение равно каналу A и инвертировано. (pin 3)
}
