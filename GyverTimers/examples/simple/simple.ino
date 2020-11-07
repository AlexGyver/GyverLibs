// Пример простой генерации прерываний аппаратным таймером
#include <GyverTimers.h>

void setup() {
  Serial.begin(9600);
  Timer1.setFrequency(3);               // Высокоточный таймер 1 для первого прерывания, частота - 3 Герца
  //Timer1.setPeriod(333333);           // то же самое! Частота 3 Гц это период 333 333 микросекунд
  //Timer1.setFrequencyFloat(4.22);     // Если нужна дробная частота в Гц  
  Timer1.enableISR();                   // Запускаем прерывание (по умолч. канал А)

  // запустим второй таймер
  Timer2.setPeriod(20000);     // Устанавливаем период таймера 20000 мкс -> 50 гц
  Timer2.enableISR(CHANNEL_A);   // Или просто .enableISR(), запускаем прерывание на канале А таймера 2
  pinMode(13, OUTPUT);           // будем мигать
}

void loop() {}

// Прерывание А таймера 1
ISR(TIMER1_A) {  // пишем  в сериал
  Serial.println("timer1");
}

// Прерывание А таймера 2
ISR(TIMER2_A) {
  // генерируем меандр 25 гц (в два раза меньше частоты), мигаем
  digitalWrite(13, !digitalRead(13));
  //Serial.println("timer2");
}
