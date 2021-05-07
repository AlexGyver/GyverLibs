// пример с использованием "внешнего" драйвера, который может быть
// подключен к расширителю портов.

// в качестве примера использую digitalWrite и родные пины
#define PIN_A 2
#define PIN_B 4
#define PIN_C 3
#define PIN_D 5

#include <GyverStepper.h>
GStepper<STEPPER4WIRE, STEPPER_VIRTUAL> stepper(2048);

void setup() {
  Serial.begin(9600);
  // выходы
  pinMode(PIN_A, 1);
  pinMode(PIN_B, 1);
  pinMode(PIN_C, 1);
  pinMode(PIN_D, 1);
  
  stepper.setRunMode(KEEP_SPEED); // режим поддержания скорости
  stepper.setSpeedDeg(100);       // в градусах/сек
  stepper.setAcceleration(500);

  // подключить свою функцию-обработчик шага
  stepper.attachStep(step);
  
  // подключить свою функцию-обработчик для управления питанием
  stepper.attachPower(pwr);

  stepper.autoPower(1);   // включаем авто выкл питания
}

// наша функция-обработчик. Будет вызываться на каждом шагу
// у STEPPER4WIRE val содержит состояния обмоток как 0bABCD
// у STEPPER2WIRE val содержит 0 или 1 как DIR, STEP нужно дёрнуть самому
void step(byte val) {
  // дёргаем вручную пины
  digitalWrite(PIN_D, val & 1);
  val >>= 1;
  digitalWrite(PIN_C, val & 1);
  val >>= 1;
  digitalWrite(PIN_B, val & 1);
  val >>= 1;
  digitalWrite(PIN_A, val & 1);
}

void pwr(bool val) {
  // тут val будет 0 или 1 в зависимости от питания. Подавай на EN
  Serial.println(val);
}

void loop() {
  stepper.tick();

  // разгон и остановка каждые 3 секунды
  static uint32_t tmr;
  if (millis() - tmr >= 3000) {
    tmr = millis();
    static bool dir = 1;
    dir = !dir;
    if (!dir) stepper.stop();
    else stepper.setSpeedDeg(100);
  }
}
