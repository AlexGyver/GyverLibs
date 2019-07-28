#include "GyverRTOS.h"
#include "LowPower.h"

#define SLEEP_PERIOD SLEEP_15MS
/*
  SLEEP_15MS - 15 ms sleep
  SLEEP_30MS - 30 ms sleep
  SLEEP_60MS - 60 ms sleep
  SLEEP_120MS - 120 ms sleep
  SLEEP_250MS - 250 ms sleep
  SLEEP_500MS - 500 ms sleep
  SLEEP_1S - 1 s sleep
  SLEEP_2S - 2 s sleep
  SLEEP_4S - 4 s sleep
  SLEEP_8S - 8 s sleep
*/

Gtask task1, task2, task3;
GRTOS myRTOS;
uint32_t buf, mainTimer;

void setup() {
  Serial.begin(115200);

  task1.attachTask(action1, 5000);
  task2.attachTask(action2, 3000);
  task3.attachTask(action3, 1000);

  myRTOS.setPeriod(SLEEP_PERIOD);
  myRTOS.attachList(myList);
}

void action1() {
  Serial.println("action1!");
}
void action2() {
  Serial.println("action2!");
}
void action3() {
  Serial.println("action3!");
}

void myList() {
  task1.tick(&buf, &mainTimer);
  task2.tick(&buf, &mainTimer);
  task3.tick(&buf, &mainTimer);

  // задержка для вывода текста (имитация задачи)
  delay(30);
}

void loop() {
  myRTOS.tick(&buf);  
  LowPower.powerDown(SLEEP_PERIOD, ADC_OFF, BOD_OFF);
  myRTOS.wake(&buf, &mainTimer);
}
