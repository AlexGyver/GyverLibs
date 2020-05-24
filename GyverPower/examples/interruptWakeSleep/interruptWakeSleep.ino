// просыпаемся по аппаратному прерыванию из sleep
#include <GyverPower.h>

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
  // в отличие от sleepDelay, ничего вызывать не нужно!
}

void loop() {
  Serial.println("go sleep");
  delay(300);
  
  // правильно будет вот тут включать прерывание
  // attachInterrupt(1, isr, FALLING);

  // спим ~8 секунд, но можем проснуться по кнопке
  power.sleep(SLEEP_8192MS);
  // тут проснулись, по кнопке или через указанный период  
  
  // а вот тут сразу отключать
  // detachInterrupt(1);

  Serial.println("wake up!");
  delay(300);
}
