// Пример генерации двухканальных прерываний на таймере с РАВНЫМ периодом, но сдвинутых по фазе
// два потока прерываний с сдвигом 180 градусов (полная инверсия)

#include <GyverTimers.h>

void setup() {
  Serial.begin(9600);

  Serial.print("Real timer frequency is : ");        // Выведем реальную частоту, реальная может отличаться от заданой (ограничено разрешением таймера)
  Serial.println(Timer1.setFrequencyFloat(2.50));    // Частота прерываний - 2.5 гц , используй .setFrequency(...) для целых чисел
  delay(1000);
  Timer1.enableISR(CHANNEL_A, 0);     // Первый канал - А, начальная фаза - 0 градусов
  Timer1.enableISR(CHANNEL_B, 180);   // Второй канал - B, начальная фаза - 180 градусов
}

void loop() {}

// два прерывания на одном таймере
ISR(TIMER1_A) {
  Serial.println(" Channel A interrupt !");   // Прерывание А
}

ISR(TIMER1_B) {
  Serial.println(" Channel B interrupt !");   // Прерывание B
}
