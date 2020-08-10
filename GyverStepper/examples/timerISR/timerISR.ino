// пример с тиком по прерыванию таймера
// используется GyverTimers

#include <GyverStepper.h>
GStepper<STEPPER4WIRE> stepper(2048, 5, 3, 4, 2);
// мотор с драйвером ULN2003 подключается по порядку пинов, но крайние нужно поменять местами
// то есть у меня подключено D2-IN1, D3-IN2, D4-IN3, D5-IN4, но в программе поменял 5 и 2

#include <GyverTimers.h>

void setup() {
  Serial.begin(115200);

  // режим следования к целевй позиции
  stepper.setRunMode(FOLLOW_POS);

  // установка макс. скорости в шагах/сек
  stepper.setMaxSpeed(400);

  // установка ускорения в шагах/сек/сек
  stepper.setAcceleration(500);

  // настраиваем прерывания с периодом, при котором 
  // система сможет обеспечить максимальную скорость мотора.
  // Для большей плавности лучше лучше взять период чуть меньше, например в два раза
  Timer2.setPeriod(stepper.getMinPeriod() / 2);

  // взводим прерывание
  Timer2.enableISR();
}

// обработчик
ISR(TIMER2_A) {
  stepper.tick(); // тикаем тут
}

void loop() {
  // просто крутим туды-сюды
  if (!stepper.tick()) {  // тут всё равно вызываем для смены направления
    static bool dir;
    dir = !dir;
    stepper.setTarget(dir ? -400 : 400);
  }

  // график положения
  Serial.println(stepper.getCurrent());

  // задержка, чтобы показать работу степпера в прерывании  
  delay(100);
}
