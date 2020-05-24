// просыпаемся по аппаратному прерыванию из sleepDelay

#include <GyverPower.h>

// sleepDelay состоит из нескольких sleep, и команда wakeUp()
// позволяет окончательно проснуться по прерыванию.
// Без неё мы будем "досыпать" некоторое время

void setup() {
  Serial.begin(9600);

  // кнопка подключена к GND и D3
  pinMode(3, INPUT_PULLUP);

  // подключаем прерывание на пин D3 (Arduino NANO)
  attachInterrupt(1, isr, FALLING);

  // глубокий сон
  power.setSleepMode(POWERDOWN_SLEEP);
}

// обработчик аппаратного прерывания
void isr() {
  // дёргаем за функцию "проснуться"
  // без неё проснёмся чуть позже (через 0-8 секунд)
  power.wakeUp();  
}

void loop() {
  Serial.println("go sleep");
  delay(300);
  
  // правильно будет вот тут включать прерывание
  // attachInterrupt(1, isr, FALLING);

  // спим 12 секунд, но можем проснуться по кнопке
  power.sleepDelay(12000);
  // тут проснулись по кнопке или через указанный период
  
  // а вот тут сразу отключать
  // detachInterrupt(1);

  Serial.println("wake up!");
  delay(300);
}
