#include <directTimers.h>
/* пример генерации нескольких прерываний по таймерам */

volatile int actualValue;

void setup() {
  Serial.begin(9600);

  TIMER1_setClock(PRESCALER_1024); // максимально замедляем таймер
  TIMER1_setMode(CTC_MODE); // включаем сброс таймера по совпадению
  TIMER1_COMPA_setValue(7811); // настроим таймер1 на прерывание 2 раза в секунду
  TIMER1_attach_COMPA(); // мигаем диодом
  pinMode(13, 1); // подключим светодиод на 13 ноге

  TIMER2_setClock(PRESCALER_1024); // максимально замедляем таймер
  TIMER2_setMode(CTC_MODE); // включаем сброс таймера по совпадению
  TIMER2_COMPA_setValue(24); // настраиваем прерывание на часототу 625 гц
  TIMER2_attach_COMPA(); // функция для получения актуального значения с ацп

  WDT_attachInterrupt(128); // настраиваем ватчдог на прерывание примерно раз в секунду >> доступны делители 16/32/64/128/256/512/1024 => ~0.125sec/0.25sec/0.5sec/1sec/2sec/4sec/8sec;
  while (millis() < 5000); // пока не пройдет 20 секунд сидим в setup и принимаем прерывания ватчдога
  WDT_detachInterrupt(); // отключаем прерывания ватчдога и продолжаем

}


void loop() {
  Serial.println(actualValue); // кидаем в порт актуальное значение с ацп
}

ISR_T2_COMPA {
  actualValue = analogRead(A0); // читаем ацп в прерывании
}

ISR_WDT {
  Serial.println("hello from watchdog"); // прерывание ватчдога
}

ISR_T1_COMPA { // прерывание мигания светодиодом
  digitalWrite(13, !digitalRead(13));
}
